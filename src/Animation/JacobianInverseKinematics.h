#ifndef _Animation_JacobianInverseKinematics_H
#define _Animation_JacobianInverseKinematics_H

#include <Animation/KinematicChain.h>
#include <Math/Matrix.h>
#include <Math/finite.h>

namespace Animation
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	JacobianInverseKinematics
	///
	/// \brief	Jacobian inverse kinematics.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	05/04/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class JacobianInverseKinematics
	{
	protected:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \typedef	double JacobianScalar
		///
		/// \brief	Defines an alias representing the jacobian scalar. double is required for precision...
		////////////////////////////////////////////////////////////////////////////////////////////////////
		typedef double JacobianScalar ;

		/// \brief	The kinematic chain.
		KinematicChain * m_chain;
		/// \brief	The extremity of the kinematic chain.
		KinematicChain::Node * m_node;
		/// \brief	The degrees of freedom of the kinematic chain.
		::std::vector<KinematicChain::DegreeOfFreedom> m_degreesOfFreedom;

	protected:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Matrix<JacobianScalar> JacobianInverseKinematics::computeJacobian(Math::Vector3f const & offset)
		///
		/// \brief	Calculates the jacobian matrix.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	offset	The offset from the extremity.
		///
		/// \return	The calculated jacobian matrix.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Matrix<JacobianScalar> computeJacobian(Math::Vector3f const & offset)
		{
			Math::Matrix<JacobianScalar> result(3, m_degreesOfFreedom.size()) ;
			for(unsigned int cpt=0 ; cpt<m_degreesOfFreedom.size() ; ++cpt)
			{
				Math::Vector3f derivate = m_chain->derivate(m_node, offset, m_degreesOfFreedom[cpt], 0.01f) ;
				for(int row=0 ; row<3 ; ++row)
				{
					result(row, cpt) = derivate[row] ;
				}
			}
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	float JacobianInverseKinematics::angleCostFunction()
		///
		/// \brief	Computes the angle cost function (tends to keep distance from limits).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	The cost.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float angleCostFunction()
		{
			float result = 0.0f ;
			for(unsigned int cpt=0 ; cpt<m_degreesOfFreedom.size() ; ++cpt)
			{
				float tmp = (m_degreesOfFreedom[cpt].constraint().middle()-m_degreesOfFreedom[cpt]) ;
				result += tmp*tmp ;
			}
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Function> float JacobianInverseKinematics::derivateCostFunction(const Function & function,
		/// 	KinematicChain::DegreeOfFreedom & dof, float epsilon)
		///
		/// \brief	Derivates the cost function.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \tparam	Function	Type of the cost function.
		/// \param	function   	The cost function.
		/// \param [in,out]	dof	The degree of freedom.
		/// \param	epsilon	   	The epsilon used for derivation.
		///
		/// \return	The derivate.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Function>
		float derivateCostFunction(const Function & function, KinematicChain::DegreeOfFreedom & dof, float epsilon)
		{
			float base = dof ;
			dof = base + epsilon ;
			float ePlus = function() ;
			dof = base - epsilon ;
			float eMinus = function() ;
			dof = base ;
			return (ePlus-eMinus)/(2.0f*epsilon) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Function> Math::Matrix<JacobianScalar> JacobianInverseKinematics::derivateCostFunction(const Function & function,
		/// 	float epsilon)
		///
		/// \brief	Derivates a cost function.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \tparam	Function	Type of the cost function.
		/// \param	function	The cost function.
		/// \param	epsilon 	The epsilon used for derivation.
		///
		/// \return	The partial derivate.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Function>
		Math::Matrix<JacobianScalar> derivateCostFunction(const Function & function, float epsilon)
		{
			Math::Matrix<JacobianScalar> result(m_degreesOfFreedom.size(), 1) ;
			for(unsigned int cpt=0 ; cpt<m_degreesOfFreedom.size() ; ++cpt)
			{
				result(cpt,0) = derivateCostFunction(function, m_degreesOfFreedom[cpt], epsilon) ;
			}
			return result ;
		}

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	InverseKinematics::InverseKinematics(KinematicChain * chain,
		/// 	KinematicChain::Node * extremity)
		///
		/// \brief	Initializes a new instance of the <see cref="InverseKinematics"/> class.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	chain	 	The kinematic chain.
		/// \param	extremity	The extremity on which inverse kimatics will be computed.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		JacobianInverseKinematics(KinematicChain * chain, KinematicChain::Node * extremity)
			: m_node(extremity)
		{
			extremity->collectDegreesOfFreedom(m_degreesOfFreedom) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Vector3f JacobianInverseKinematics::extremityPosition(const Math::Vector3f & offset=Math::makeVector(0.0f,
		/// 	0.0f,0.0f)) const
		///
		/// \brief	Extremity position.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	offset	(optional) the offset.
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Vector3f extremityPosition(const Math::Vector3f & offset=Math::makeVector(0.0f,0.0f,0.0f)) const
		{
			return m_node->getGlobalTransformation()*offset ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const ::std::vector<KinematicChain::DegreeOfFreedom> & JacobianInverseKinematics::getDOF() const
		///
		/// \brief	Gets the degrees of freedom.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	The degrees of freedom.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ::std::vector<KinematicChain::DegreeOfFreedom> & getDOF() const
		{
			return m_degreesOfFreedom ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	::std::vector<KinematicChain::DegreeOfFreedom> & JacobianInverseKinematics::getDOF()
		///
		/// \brief	Gets the degrees of freedom.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	The degrees of freedom.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		::std::vector<KinematicChain::DegreeOfFreedom> & getDOF()
		{
			return m_degreesOfFreedom ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void InverseKinematics::convergeToward(Math::Vector3f const & target,
		/// 	float maxDeltaAngle=::std::numeric_limits<float>::max(),
		/// 	Math::Vector3f const & offset=Math::Vector3f())
		///
		/// \brief	Converge toward the provided target.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	13/02/2016
		///
		/// \param	target		 	Target for the inverse kinematic.
		/// \param	maxDeltaAngle	(optional) the maximum angle variation that can be applied.
		/// \param	offset		 	(optional) the offset from the extremity of the chain.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		double convergeToward(Math::Vector3f const & target, float maxDeltaAngle=::std::numeric_limits<float>::max(), Math::Vector3f const & offset=Math::makeVector(0.0f,0.0f,0.0f))
		{
			int failures = 0 ;
			bool runAgain = false ;
			do
			{
				if(runAgain) 
				{
					++failures ;
					::std::cout<<"Failure "<<failures<<::std::endl ;
				}
				runAgain = false ;

				Math::Matrix<JacobianScalar> jacobian = computeJacobian(offset) ;
				Math::Matrix<JacobianScalar> dx = target-m_node->getGlobalTransformation()*offset ;
				Math::Matrix<JacobianScalar> pseudoInverse = jacobian.pseudoInverse() ;
				Math::Matrix<JacobianScalar> kernel = Math::Matrix<JacobianScalar>::identity(m_degreesOfFreedom.size(), m_degreesOfFreedom.size())-pseudoInverse*jacobian ;
				Math::Matrix<JacobianScalar> dz = derivateCostFunction([this]() -> float { return angleCostFunction() ; }, 0.001f)*-1.0 ;
				Math::Matrix<JacobianScalar> dTheta = pseudoInverse*dx + (kernel*(dz))*0.01 ;
				
				//JacobianScalar max = ::std::max(fabs(dTheta.maxValue()), fabs(dTheta.minValue())) ;
				//dTheta = dTheta/max*maxDeltaAngle ;

				for(unsigned int row = 0 ; row<dTheta.rows() ; ++row)
				{
					JacobianScalar dAngle = Math::makeInterval<JacobianScalar>(-maxDeltaAngle, maxDeltaAngle).clamp(dTheta(row,0)) ;
					if(!Math::is_valid(float(dAngle)))
					{
						dAngle = Math::makeInterval(-maxDeltaAngle, maxDeltaAngle).random();
						runAgain = true ;
					}
					m_degreesOfFreedom[row] = m_degreesOfFreedom[row]+float(dAngle) ;
				}

			} while(runAgain) ;

			return (target-m_node->getGlobalTransformation()*offset).norm() ;			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Function> double JacobianInverseKinematics::convergeToward(const Function & secondaryTaskCostFunction,
		/// 	Math::Vector3f const & target, float maxDeltaAngle=::std::numeric_limits<float>::max(),
		/// 	Math::Vector3f const & offset=Math::makeVector(0.0f,0.0f,0.0f))
		///
		/// \brief	Converge toward.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \tparam	Function	Type of the function.
		/// \param	secondaryTaskCostFunction	The secondary task cost function. This cost function is automatically derived with the degrees of freedom.
		/// \param	target		 	Target for the inverse kinematic.
		/// \param	maxDeltaAngle	(optional) the maximum angle variation that can be applied.
		/// \param	offset		 	(optional) the offset from the extremity of the chain.
		///
		/// \return	The distance between the extremity and the target.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Function>
		double convergeToward(const Function & secondaryTaskCostFunction, Math::Vector3f const & target, float maxDeltaAngle=::std::numeric_limits<float>::max(), Math::Vector3f const & offset=Math::makeVector(0.0f,0.0f,0.0f))
		{
			int failures = 0 ;
			bool runAgain = false ;
			do
			{
				if(runAgain) 
				{
					++failures ;
					::std::cout<<"Failure "<<failures<<::std::endl ;
				}
				runAgain = false ;

				Math::Matrix<JacobianScalar> jacobian = computeJacobian(offset) ;
				Math::Matrix<JacobianScalar> dx = target-m_node->getGlobalTransformation()*offset ;
				Math::Matrix<JacobianScalar> pseudoInverse = jacobian.pseudoInverse() ;
				Math::Matrix<JacobianScalar> kernel = Math::Matrix<JacobianScalar>::identity(m_degreesOfFreedom.size(), m_degreesOfFreedom.size())-pseudoInverse*jacobian ;
				Math::Matrix<JacobianScalar> dz = derivateCostFunction(secondaryTaskCostFunction, 0.001f)*-1.0 ;
				 
				Math::Matrix<JacobianScalar> dTheta = pseudoInverse*dx + (kernel*(dz))/**0.01*/ ;

				for(unsigned int row = 0 ; row<dTheta.rows() ; ++row)
				{
					JacobianScalar dAngle = Math::makeInterval<JacobianScalar>(-maxDeltaAngle, maxDeltaAngle).clamp(dTheta(row,0)) ;
					if(!Math::is_valid(float(dAngle))) // If value is invalid, we randomize...
					{
						dAngle = Math::makeInterval(-maxDeltaAngle, maxDeltaAngle).random();
						runAgain = true ;
					}
					m_degreesOfFreedom[row] = m_degreesOfFreedom[row]+float(dAngle) ;
				}

			} while(runAgain) ;

			return (target-m_node->getGlobalTransformation()*offset).norm() ;			
		}
	};
}

#endif