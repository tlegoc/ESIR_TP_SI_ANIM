#ifndef _Math_Internal_JacobiSVD_H
#define _Math_Internal_JacobiSVD_H

#include <Eigen/Dense>

namespace Math
{
	namespace Internal
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	JacobiSVD
		///
		/// \brief	Extension of the Jacobi singular value decomposition provided by Eigen library. This 
		/// 		extension enables the computation of a pseudo inverse (Penrose Moore) matrix.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \tparam	MatrixType	Type of the used matrix (Eigen type)
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class MatrixType>
		class JacobiSVD : public Eigen::JacobiSVD<MatrixType>
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	JacobiSVD::JacobiSVD(const MatrixType & matrix) : Eigen::JacobiSVD<MatrixType>(matrix,
			/// 	Eigen::ComputeFullU | Eigen::ComputeFullV)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	04/04/2016
			///
			/// \param	matrix	The matrix.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			JacobiSVD(const MatrixType & matrix)
				: Eigen::JacobiSVD<MatrixType>(matrix, Eigen::ComputeThinU | Eigen::ComputeThinV)
			{}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void JacobiSVD::pseudoInverse( MatrixType& pseudoInverseMatrix) const
			///
			/// \brief	Pseudo inverse of the current matrix.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	04/04/2016
			///
			/// \param [out]	pseudoInverseMatrix	The computed pseudo inverse.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void pseudoInverse(MatrixType & pseudoInverseMatrix) const
			{
				eigen_assert(m_isInitialized && "SVD is not initialized.");
				double  pinvtoler=1.e-6; // choose your tolerance wisely!
				SingularValuesType singularValues_inv=m_singularValues;
				for ( long i=0; i<m_workMatrix.cols(); ++i) {
					if ( m_singularValues(i) > pinvtoler )
						singularValues_inv(i)=typename MatrixType::RealScalar(1.0)/m_singularValues(i);
					else singularValues_inv(i)=0;
				}
				pseudoInverseMatrix= (m_matrixV*singularValues_inv.asDiagonal()*m_matrixU.transpose());
			}
		};
	}
}

#endif