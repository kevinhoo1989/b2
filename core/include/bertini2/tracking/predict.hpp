//This file is part of Bertini 2.
//
//tracking/predict.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//tracking/predict.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with tracking/predict.hpp.  If not, see <http://www.gnu.org/licenses/>.
//

// Copyright(C) 2015, 2016 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license, 
// as well as COPYING.  Bertini2 is provided with permitted 
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// daniel brake, university of notre dame

/**
\file predict.hpp 

\brief Wrapper functions for calling ODE predictors for systems.
*/

#ifndef BERTINI_PREDICT_HPP
#define BERTINI_PREDICT_HPP

#include "bertini2/tracking/ode_predictors.hpp"

namespace bertini{
	namespace tracking{

		/**
		Wrapper class for calling an ODE predictor, using fixed precision

		\param predictor_choice The enum class selecting the predictor to be used.
		\param next_space The computed prediction.
		\param sys The system being solved.
		\param current_space The current space variable vector.
		\param current_time The current time.
		\param delta_t The size of the time step.
		\param condition_number_estimate The computed estimate of the condition number of the Jacobian.
		\param num_steps_since_last_condition_number_computation.  Updated in this function.
		\param frequency_of_CN_estimation How many steps to take between condition number estimates.
		\param prec_type The operating precision type.  
		\param tracking_tolerance How tightly to track the path.
		\param AMP_config The settings for adaptive multiple precision.

		\tparam ComplexType The complex number type for evaluation.
		\tparam RealType The complex number type for evaluation.
		*/
		template <typename ComplexType, typename RealType>
		SuccessCode Predict(config::Predictor predictor_choice,
							Vec<ComplexType> & next_space,
							   System const& sys,
							   Vec<ComplexType> const& current_space, ComplexType current_time, 
							   ComplexType const& delta_t,
							   RealType & condition_number_estimate,
							   unsigned & num_steps_since_last_condition_number_computation, 
							   unsigned frequency_of_CN_estimation, 
							   RealType const& tracking_tolerance)
		{
			static_assert(std::is_same<typename Eigen::NumTraits<RealType>::Real, typename Eigen::NumTraits<ComplexType>::Real>::value,"underlying complex type and the type for comparisons must match");
			
			predict::ExplicitRKPredictor predictor(predictor_choice);

			return predictor.Predict(next_space,
									sys,
									current_space, current_time, 
									delta_t,
									condition_number_estimate,
									num_steps_since_last_condition_number_computation, 
									frequency_of_CN_estimation, 
									tracking_tolerance);
		}







		




		/**
		An overload of Predict which returns (by reference) error estimate, and estimates of the norm of \f$J\f$ and \f$J^{-1}\f$ from AMP2 paper \cite AMP2.

		\see Predict
		*/
		template <typename ComplexType, typename RealType>
		SuccessCode Predict(config::Predictor predictor_choice,
							Vec<ComplexType> & next_space,
							RealType & size_proportion, /*\f$a\f$ from the AMP2 paper */
							RealType & norm_J,
							RealType & norm_J_inverse,
							System const& sys,
							Vec<ComplexType> const& current_space, ComplexType current_time, 
							ComplexType const& delta_t,
							RealType & condition_number_estimate,
							unsigned & num_steps_since_last_condition_number_computation, 
							unsigned frequency_of_CN_estimation, 
							RealType const& tracking_tolerance,
							config::AdaptiveMultiplePrecisionConfig const& AMP_config)
		{
			static_assert(std::is_same<typename Eigen::NumTraits<RealType>::Real, typename Eigen::NumTraits<ComplexType>::Real>::value,"underlying complex type and the type for comparisons must match");
			
			predict::ExplicitRKPredictor<ComplexType, RealType> predictor(predictor_choice);

			return predictor.Predict(next_space,
										  size_proportion,
										  norm_J,
										  norm_J_inverse,
										sys,
										current_space, current_time, 
										delta_t,
										condition_number_estimate,
										num_steps_since_last_condition_number_computation, 
										frequency_of_CN_estimation, 
										tracking_tolerance,
										AMP_config);
		}







		/**
		An overload of Predict which returns (by reference) error estimate, and estimates of the norm of \f$J\f$ and \f$J^{-1}\f$, and the size proportion \f$a\f$ from AMP2 paper \cite AMP2.

		\see Predict
		*/
		template <typename ComplexType, typename RealType>
		SuccessCode Predict(config::Predictor predictor_choice,
							Vec<ComplexType> & next_space,
							RealType & error_estimate,
							RealType & size_proportion, /*\f$a\f$ from the AMP2 paper */
							RealType & norm_J,
							RealType & norm_J_inverse,
							System const& sys,
							Vec<ComplexType> const& current_space, ComplexType current_time, 
							ComplexType const& delta_t,
							RealType & condition_number_estimate,
							unsigned & num_steps_since_last_condition_number_computation, 
							unsigned frequency_of_CN_estimation, 
							RealType const& tracking_tolerance,
							config::AdaptiveMultiplePrecisionConfig const& AMP_config)
		{
			static_assert(std::is_same<typename Eigen::NumTraits<RealType>::Real, typename Eigen::NumTraits<ComplexType>::Real>::value,"underlying complex type and the type for comparisons must match");

			predict::ExplicitRKPredictor<ComplexType, RealType> predictor(predictor_choice);

			return predictor.Predict(next_space,
												error_estimate,
												size_proportion,
												norm_J,
												norm_J_inverse,
												sys,
												current_space, current_time, 
												delta_t,
												condition_number_estimate,
												num_steps_since_last_condition_number_computation, 
												frequency_of_CN_estimation, 
												tracking_tolerance,
												AMP_config);
		}



		/**
		Wrapper class for calling an ODE predictor, using adaptive precision, not returning some meta-data about the step.

		\param predictor_choice The enum class selecting the predictor to be used.
		\param next_space The computed prediction.
		\param sys The system being solved.
		\param current_space The current space variable vector.
		\param current_time The current time.
		\param delta_t The size of the time step.
		\param condition_number_estimate The computed estimate of the condition number of the Jacobian.
		\param num_steps_since_last_condition_number_computation.  Updated in this function.
		\param frequency_of_CN_estimation How many steps to take between condition number estimates.
		\param prec_type The operating precision type.  
		\param tracking_tolerance How tightly to track the path.
		\param AMP_config The settings for adaptive multiple precision.

		\tparam ComplexType The complex number type for evaluation.
		\tparam RealType The complex number type for evaluation.
		*/
		template <typename ComplexType, typename RealType>
		SuccessCode Predict(config::Predictor predictor_choice,
							Vec<ComplexType> & next_space,
							   System const& sys,
							   Vec<ComplexType> const& current_space, ComplexType current_time, 
							   ComplexType const& delta_t,
							   RealType & condition_number_estimate,
							   unsigned & num_steps_since_last_condition_number_computation, 
							   unsigned frequency_of_CN_estimation, 
							   RealType const& tracking_tolerance,
							   config::AdaptiveMultiplePrecisionConfig const& AMP_config)
		{
			static_assert(std::is_same<typename Eigen::NumTraits<RealType>::Real, typename Eigen::NumTraits<ComplexType>::Real>::value,"underlying complex type and the type for comparisons must match");

			RealType size_proportion, norm_J, norm_J_inverse;

			return Predict(predictor_choice,
			               next_space,
							size_proportion,
							norm_J,
							norm_J_inverse,
							sys,
							current_space, current_time, 
							delta_t,
							condition_number_estimate,
							num_steps_since_last_condition_number_computation, 
							frequency_of_CN_estimation, 
							tracking_tolerance,
							AMP_config);
		}


		



		

	} // re: namespace tracking

	
} // re: namespace bertini



#endif
