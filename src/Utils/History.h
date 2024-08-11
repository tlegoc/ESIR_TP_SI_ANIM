#ifndef _Utils_history_H
#define _Utils_history_H

namespace Utils
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	History
	///
	/// \brief	History of a value, at least previous value, current value, next value. The number of
	/// 		previous values can be user provided.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	01/03/2016
	///
	/// \tparam	Type	Type of the elements stored in the history.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Type>
	class History
	{
	protected:
		/// \brief	The history.
		::std::vector<Type> m_history ;
		/// \brief	The current value index in the history.
		int m_currentIndex ;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	History::History(Type const & initializer, int memory=1)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		///
		/// \param	initializer	The initializer.
		/// \param	memory	   	(optional) Number of previous values that should be stored in this history.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		History(Type const & initializer = Type(), int memory=1)
			: m_history(memory+2, initializer), m_currentIndex(0)
		{
			assert(memory>=1) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Type & History::current()
		///
		/// \brief	Gets the current value.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		///
		/// \return	The current value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Type & current() 
		{ return m_history[m_currentIndex] ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Type & History::current() const
		///
		/// \brief	Gets the current.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		///
		/// \return	The current value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Type & current() const
		{ return m_history[m_currentIndex] ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Type & History::next()
		///
		/// \brief	Gets the next value.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		///
		/// \return	The next value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Type & next()
		{ return m_history[(m_currentIndex+1)%(int)m_history.size()] ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Type & History::next() const
		///
		/// \brief	Gets the next value.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		///
		/// \return	The next value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Type & next() const
		{ return m_history[(m_currentIndex+1)%(int)m_history.size()] ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Type & History::previous(int index) const
		///
		/// \brief	Returns the previous value. The optional parameter from one to the memory value 
		/// 		(minimum 1).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		///
		/// \param	index	Zero-based index of the.
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Type & previous(int index=1) const
		{ 
			assert(index>=1) ;
			assert(index<m_history.size()-2) ;
			return m_history[(m_currentIndex+m_history.size()-index)%(int)m_history.size()] ; 
		}		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Type & History::previous(int index) const
		///
		/// \brief	Returns the previous value. The optional parameter from one to the memory value 
		/// 		(minimum 1).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		///
		/// \param	index	Zero-based index of the.
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Type & previous(int index=1)
		{ 
			assert(index>=1) ;
			assert(index<=(int)m_history.size()-2) ;
			return m_history[(m_currentIndex+m_history.size()-index)%(int)m_history.size()] ; 
		}		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void History::commit()
		///
		/// \brief	Commits the current value. Current value becomes previous value, next value becomes 
		/// 		current value etc...
		///
		/// \warning The next value is in an undefined state.
		/// 
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void commit() 
		{ m_currentIndex = (m_currentIndex+1)%(int)m_history.size() ; }
	};
}

#endif