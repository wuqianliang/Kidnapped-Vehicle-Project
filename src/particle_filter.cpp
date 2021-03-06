/*
 * particle_filter.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 *  Modified on: Feb 23,2018
 *      Author: Qianliang Wu
 */

#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

#include "particle_filter.h"

using namespace std;

static default_random_engine gen;

void ParticleFilter::init( double x, double y, double theta, double std[] )
{
	/*
	 * TODO: Set the number of particles. Initialize all particles to first position (based on estimates of
	 *   x, y, theta and their uncertainties from GPS) and all weights to 1.
	 * Add random Gaussian noise to each particle.
	 * NOTE: Consult particle_filter.h for more information about this method (and others in this file).
	 */
	num_particles = 100;

	/* define normal distributions for sensor noise */
	normal_distribution<double>	N_x_init( 0, std[0] );
	normal_distribution<double>	N_y_init( 0, std[1] );
	normal_distribution<double>	N_theta_init( 0, std[2] );

	/* init particles */
	for ( int i = 0; i < num_particles; i++ )
	{
		Particle p;
		p.id		= i;
		p.x		= x;
		p.y		= y;
		p.theta		= theta;
		p.weight	= 1.0;

		/* add noise */
		p.x	+= N_x_init( gen );
		p.y	+= N_y_init( gen );
		p.theta += N_theta_init( gen );

		particles.push_back( p );
	}

	is_initialized = true;
}


void ParticleFilter::prediction( double delta_t, double std_pos[], double velocity, double yaw_rate )
{
	/*
	 * TODO: Add measurements to each particle and add random Gaussian noise.
	 * NOTE: When adding noise you may find std::normal_distribution and std::default_random_engine useful.
	 *  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
	 *  http://www.cplusplus.com/reference/random/default_random_engine/
	 * define normal distributions for sensor noise
	 */
	normal_distribution<double>	N_x( 0, std_pos[0] );
	normal_distribution<double>	N_y( 0, std_pos[1] );
	normal_distribution<double>	N_theta( 0, std_pos[2] );

	for ( int i = 0; i < num_particles; i++ )
	{
		/* calculate new state */
		if ( fabs( yaw_rate ) < 0.00001 )
		{
			particles[i].x	+= velocity * delta_t * cos( particles[i].theta );
			particles[i].y	+= velocity * delta_t * sin( particles[i].theta );
		}else  {
			particles[i].x		+= velocity / yaw_rate * (sin( particles[i].theta + yaw_rate * delta_t ) - sin( particles[i].theta ) );
			particles[i].y		+= velocity / yaw_rate * (cos( particles[i].theta ) - cos( particles[i].theta + yaw_rate * delta_t ) );
			particles[i].theta	+= yaw_rate * delta_t;
		}

		/* add noise */
		particles[i].x		+= N_x( gen );
		particles[i].y		+= N_y( gen );
		particles[i].theta	+= N_theta( gen );
	}
}


void ParticleFilter::dataAssociation( std::vector<LandmarkObs> & predicted, std::vector<LandmarkObs> & observations )
{
	/*
	 * TODO: Find the predicted measurement that is closest to each observed measurement and assign the
	 *   observed measurement to this particular landmark.
	 * NOTE: this method will NOT be called by the grading code. But you will probably find it useful to
	 *   implement this method and use it as a helper during the updateWeights phase.
	 */


	for ( unsigned int i = 0; i < observations.size(); i++ )
	{

		/* init minimum distance to maximum possible */
		double min_dist = numeric_limits<double>::max();

		/* init id of landmark from map placeholder to be associated with the observation */
		int	map_id	= -1;
		double	ass_x	= 0.0;
		double	ass_y	= 0.0;
		for ( unsigned int j = 0; j < predicted.size(); j++ )
		{

			/* get distance between current/predicted landmarks */
			double cur_dist = dist( observations[i].x, observations[i].y, predicted[j].x, predicted[j].y );

			/* find the predicted landmark nearest the current observed landmark */
			if ( cur_dist < min_dist )
			{
				min_dist	= cur_dist;
				map_id		= predicted[j].id;
				ass_x		= predicted[j].x;
				ass_y		= predicted[j].y;
			}
		}

		/* set the observation's id to the nearest predicted landmark's id */
		observations[i].id		= map_id;
		observations[i].associated_x	= ass_x;
		observations[i].associated_y	= ass_y;
	}
}


void ParticleFilter::updateWeights( double sensor_range, double std_landmark[],
				    const std::vector<LandmarkObs> &observations, const Map &map_landmarks )
{
	/*
	 * TODO: Update the weights of each particle using a mult-variate Gaussian distribution. You can read
	 *   more about this distribution here: https://en.wikipedia.org/wiki/Multivariate_normal_distribution
	 * NOTE: The observations are given in the VEHICLE'S coordinate system. Your particles are located
	 *   according to the MAP'S coordinate system. You will need to transform between the two systems.
	 *   Keep in mind that this transformation requires both rotation AND translation (but no scaling).
	 *   The following is a good resource for the theory:
	 *   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
	 *   and the following is a good resource for the actual equation to implement (look at equation
	 *   3.33
	 *   http://planning.cs.uiuc.edu/node99.html
	 */

	double	s_x	= std_landmark[0];
	double	s_y	= std_landmark[1];
	/* for each particle... */
	for ( int i = 0; i < num_particles; i++ )
	{
		/* get the particle x, y coordinates */
		double	p_x	= particles[i].x;
		double	p_y	= particles[i].y;
		double	p_theta = particles[i].theta;

		/* create a vector to hold the map landmark locations predicted to be within sensor range of the particle */
		vector<LandmarkObs> predictions;

		/* for each map landmark*/
		for ( unsigned int j = 0; j < map_landmarks.landmark_list.size(); j++ )
		{
			/* get id and x,y coordinates */
			float	lm_x	= map_landmarks.landmark_list[j].x_f;
			float	lm_y	= map_landmarks.landmark_list[j].y_f;
			int	lm_id	= map_landmarks.landmark_list[j].id_i;

			/*
			 * only consider landmarks within sensor range of the particle 
			 */
			if ( fabs( lm_x - p_x ) <= sensor_range && fabs( lm_y - p_y ) <= sensor_range )
			{
				/* add prediction to vector */
				predictions.push_back( LandmarkObs { lm_id, lm_x, lm_y, 0.0, 0.0 } );
			}
		}

		/* create and populate a copy of the list of observations transformed from vehicle coordinates to map coordinates */
		vector<LandmarkObs>	transformed_os;
		double			cos_theta	= cos( p_theta );
		double			sin_theta	= sin( p_theta );

		for ( unsigned int j = 0; j < observations.size(); j++ )
		{
			double	t_x	= cos_theta * observations[j].x - sin_theta * observations[j].y + p_x;
			double	t_y	= sin_theta * observations[j].x + cos_theta * observations[j].y + p_y;
			transformed_os.push_back( LandmarkObs { observations[j].id, t_x, t_y, 0.0, 0.0 } );
		}

		/* perform dataAssociation for the predictions and transformed observations on current particle */
		dataAssociation( predictions, transformed_os );

		/* reinit weight */
		particles[i].weight = 1.0;

		for ( unsigned int j = 0; j < transformed_os.size(); j++ )
		{
			/* placeholders for observation and associated prediction coordinates */
			double	o_x	= transformed_os[j].x;
			double	o_y	= transformed_os[j].y;

			double	pr_x	= transformed_os[j].associated_x;
			double	pr_y	= transformed_os[j].associated_y;

			int associated_prediction = transformed_os[j].id;

			/* calculate weight for this observation with multivariate Gaussian */

			double obs_w = (1 / (2 * M_PI * s_x * s_y) ) * exp( -(pow( pr_x - o_x, 2 ) / (2 * pow( s_x, 2 ) ) + (pow( pr_y - o_y, 2 ) / (2 * pow( s_y, 2 ) ) ) ) );

			/* product of this obersvation weight with total observations weight */
			particles[i].weight *= obs_w;
		}
		weights.push_back( particles[i].weight );
	}
}


void ParticleFilter::resample()
{
	/*
	 * TODO: Resample particles with replacement with probability proportional to their weight.
	 * NOTE: You may find std::discrete_distribution helpful here.
	 *   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
	 */

	std::discrete_distribution<> dist( weights.begin(), weights.end() );

	/* create resampled particles */
	vector<Particle> resampled_particles;
	resampled_particles.resize( num_particles );

	/* resample the particles according to weights */
	for ( int i = 0; i < num_particles; i++ )
	{
		int idx = dist( gen );
		resampled_particles[i] = particles[idx];
	}

	/* assign the resampled_particles to the previous particles */
	//particles = resampled_particles;
	particles = std::move(resampled_particles);

	/* clear the weight vector for the next round */
	weights.clear();
}


Particle ParticleFilter::SetAssociations( Particle & particle, const std::vector<int> & associations,
					  const std::vector<double> & sense_x, const std::vector<double> & sense_y )
{
	/*
	 * particle: the particle to assign each listed association, and association's (x,y) world coordinates mapping to
	 * associations: The landmark id that goes along with each listed association
	 * sense_x: the associations x mapping already converted to world coordinates
	 * sense_y: the associations y mapping already converted to world coordinates
	 */

	particle.associations	= associations;
	particle.sense_x	= sense_x;
	particle.sense_y	= sense_y;
}


string ParticleFilter::getAssociations( Particle best )
{
	vector<int>	v = best.associations;
	stringstream	ss;
	copy( v.begin(), v.end(), ostream_iterator<int>( ss, " " ) );
	string s = ss.str();
	s = s.substr( 0, s.length() - 1 ); /* get rid of the trailing space */
	return(s);
}


string ParticleFilter::getSenseX( Particle best )
{
	vector<double>	v = best.sense_x;
	stringstream	ss;
	copy( v.begin(), v.end(), ostream_iterator<float>( ss, " " ) );
	string s = ss.str();
	s = s.substr( 0, s.length() - 1 ); /* get rid of the trailing space */
	return(s);
}


string ParticleFilter::getSenseY( Particle best )
{
	vector<double>	v = best.sense_y;
	stringstream	ss;
	copy( v.begin(), v.end(), ostream_iterator<float>( ss, " " ) );
	string s = ss.str();
	s = s.substr( 0, s.length() - 1 ); /* get rid of the trailing space */
	return(s);
}
