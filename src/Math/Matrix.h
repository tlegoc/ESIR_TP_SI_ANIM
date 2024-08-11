#ifndef _Math_Matrix_H
#define _Math_Matrix_H

#include <Eigen/Dense>
#include <Math/Internal/JacobiSVD.h>
#include <Math/Vector.h>

namespace Math
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Matrix
	///
	/// \brief	Dynamic matrix class.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	04/04/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Scalar>
	class Matrix
	{
	protected:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \typedef	Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> InternalMatrix
		///
		/// \brief	Defines an alias representing the internal matrix type (Eigen dense matrix).
		////////////////////////////////////////////////////////////////////////////////////////////////////
		typedef Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> InternalMatrix ;

		/// \brief	The matrix.
		InternalMatrix m_matrix ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix::Matrix(const InternalMatrix & matrix)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	matrix	The matrix (internal representation).
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix(const InternalMatrix & matrix)
			: m_matrix(matrix)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix::Matrix(InternalMatrix && matrix)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param [in,out]	matrix	The matrix (internal representation).
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix(InternalMatrix && matrix)
			: m_matrix(::std::move(matrix))
		{}

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix::Matrix(unsigned int rows, unsigned int columns)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	rows   	The number of rows.
		/// \param	columns	The number of columns.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix(unsigned int rows, unsigned int columns)
			: m_matrix(rows, columns)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Float, int dim> Matrix::Matrix(const Vector<Float, dim> & v)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \tparam	Float  	Type of the float.
		/// \tparam	int dim	Type of the int dim.
		/// \param	v	The const Vector&lt;Float,dim&gt; &amp; to process.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Float, int dim>
		Matrix(const Vector<Float, dim> & v)
			: m_matrix(dim,1)
		{
			for(int row=0 ; row<dim ; ++row)
			{
				m_matrix(row,0) = v[row] ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix::Matrix(const Matrix & matrix)
		///
		/// \brief	Copy constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	matrix	The matrix.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix(const Matrix & matrix)
			: m_matrix(matrix.m_matrix)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix::Matrix(Matrix && matrix)
		///
		/// \brief	Move constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param [in,out]	matrix	The matrix.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix(Matrix && matrix)
			: m_matrix(::std::move(matrix.m_matrix))
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	unsigned int Matrix::rows() const
		///
		/// \brief	Gets the number of rows.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	the number of rows.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned int rows() const
		{
			return m_matrix.rows() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	unsigned int Matrix::columns() const
		///
		/// \brief	Gets the number of columns.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	the number of columns..
		////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned int columns() const
		{
			return m_matrix.cols() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Scalar & Matrix::operator() (unsigned int row, unsigned int column)
		///
		/// \brief	 Access an elemnt of the matrix.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param row The row number (starting at 0)
		/// \param col The column number (starting at 0)
		/// \return	The value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Scalar & operator() (unsigned int row, unsigned int column)
		{
			return m_matrix(row, column) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Scalar Matrix::operator() (unsigned int row, unsigned int column) const
		///
		/// \brief	 casting operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param row The row number (starting at 0)
		/// \param col The column number (starting at 0)
		/// \return	The value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Scalar operator() (unsigned int row, unsigned int column) const
		{
			return m_matrix(row, column) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::operator* (const Matrix & matrix) const
		///
		/// \brief	Multiplication operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	matrix	The matrix.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////=
		Matrix operator* (const Matrix & matrix) const
		{
			return Matrix(m_matrix * matrix.m_matrix) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::operator* (Scalar scalar) const
		///
		/// \brief	Multiplication operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	scalar	The scalar.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix operator* (Scalar scalar) const
		{
			return Matrix(m_matrix * scalar) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::operator/ (Scalar scalar) const
		///
		/// \brief	Division operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	scalar	The scalar.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix operator/ (Scalar scalar) const
		{
			return m_matrix / scalar ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::operator+ (const Matrix & matrix) const
		///
		/// \brief	Addition operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	matrix	The matrix.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix operator+ (const Matrix & matrix) const
		{
			return Matrix(m_matrix+matrix.m_matrix) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::operator- (const Matrix & matrix) const
		///
		/// \brief	Negation operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	matrix	The matrix.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix operator- (const Matrix & matrix) const
		{
			return Matrix(m_matrix-matrix.m_matrix) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::getRow(unsigned int row) const
		///
		/// \brief	Gets a row.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	row	The row.
		///
		/// \return	The row.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix getRow(unsigned int row) const
		{
			return m_matrix.row(row) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::getColumn(unsigned int column) const
		///
		/// \brief	Gets a column.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	column	The column.
		///
		/// \return	The column.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix getColumn(unsigned int column) const
		{
			return m_matrix.col(column) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool Matrix::hasNan() const
		///
		/// \brief	Query if this object has NaN.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	true if nan, false if not.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool hasNan() const
		{
			return m_matrix.hasNaN() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool Matrix::operator!=(const Matrix & matrix) const
		///
		/// \brief	Inequality operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	matrix	The matrix.
		///
		/// \return	true if the parameters are not considered equivalent.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool operator!=(const Matrix & matrix) const
		{
			return m_matrix!=matrix.m_matrix ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool Matrix::operator==(const Matrix & matrix) const
		///
		/// \brief	Equality operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	matrix	The matrix.
		///
		/// \return	true if the parameters are considered equivalent.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool operator==(const Matrix & matrix) const
		{
			return m_matrix==matrix.m_matrix ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::transpose() const
		///
		/// \brief	Gets the transpose.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix transpose() const
		{
			return m_matrix.transpose() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Matrix Matrix::pseudoInverse() const
		///
		/// \brief	Gets the pseudo inverse.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Matrix pseudoInverse() const
		{
			Internal::JacobiSVD<InternalMatrix> svd(m_matrix) ;
			Matrix result(m_matrix.cols(), m_matrix.rows()) ;
			svd.pseudoInverse(result.m_matrix) ;
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Float, int dim> operator Math::Vector<Float, Matrix::dim> () const
		///
		/// \brief	Conversion operator
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \tparam	Float  	Type of the float.
		/// \tparam	int dim	Type of the int dim.
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Float, int dim>
		operator Math::Vector<Float, dim> () const
		{
			assert(rows()==dim) ;
			assert(columns()==1) ;
			Math::Vector<float, dim> result ;
			for(int row=0 ; row<dim ; ++row)
			{
				result[row] = (Float)m_matrix(row, 0) ;
			}
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Float, int dim> Matrix & Matrix::operator= (const Vector<Float, dim> & v)
		///
		/// \brief	Assignment operator with a vector.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \tparam	Float  	Type of the float.
		/// \tparam	int dim	Type of the int dim.
		/// \param	v	The const Vector&lt;Float,dim&gt; &amp; to process.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Float, int dim>
		Matrix & operator = (const Vector<Float, dim> & v)
		{
			m_matrix.resize(dim,1) ;
			for(int row=0 ; row<dim ; ++row)
			{
				m_matrix(row,0) = v[row] ;
			}
		}

		Matrix & operator= (const Matrix & matrix)
		{
			m_matrix = matrix.m_matrix ;
		}

		Matrix & operator=(Matrix && matrix)
		{
			m_matrix.swap(matrix) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Scalar Matrix::maxValue() const
		///
		/// \brief	Gets the maximum value of the elements in the matrix.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Scalar maxValue() const
		{
			Scalar maxValue = ::std::numeric_limits<Scalar>::min() ;
			for(unsigned int row = 0 ; row<rows() ; ++row)
			{
				for(unsigned int column = 0 ; column<columns() ; ++column)
				{
					maxValue = ::std::max(m_matrix(row,column), maxValue) ;
				}
			}
			return maxValue ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Scalar Matrix::minValue() const
		///
		/// \brief	Gets the minimum value of the elements in the matrix.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Scalar minValue() const
		{
			Scalar minValue = ::std::numeric_limits<Scalar>::max() ;
			for(unsigned int row = 0 ; row<rows() ; ++row)
			{
				for(unsigned int column = 0 ; column<columns() ; ++column)
				{
					minValue = ::std::min(m_matrix(row,column), minValue) ;
				}
			}
			return minValue ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static Matrix Matrix::identity(unsigned int rows, unsigned int columns)
		///
		/// \brief	Returns an identity matrix.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	05/04/2016
		///
		/// \param	rows   	The rows.
		/// \param	columns	The columns.
		///
		/// \return	An identity matrix.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static Matrix identity(unsigned int rows, unsigned int columns)
		{
			return Matrix(InternalMatrix::Identity(rows, columns)) ;
		}
	};

}

#endif