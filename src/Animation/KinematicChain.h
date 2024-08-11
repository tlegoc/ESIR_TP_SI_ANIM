#ifndef _Animation_Kinematics_H
#define _Animation_Kinematics_H

#include <Math/Matrix4x4f.h>
#include <Math/Vectorf.h>
#include <Math/Interval.h>
#include <cassert>
#include <vector>
#include <iterator>

namespace Animation
{

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	KinematicChain
	///
	/// \brief	An articulated chain.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	11/02/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class KinematicChain
	{
	public:
		// Forward declarations of some classes
		class Node ;
		class StaticNode ;
		class DynamicNode ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	DegreeOfFreedom
		///
		/// \brief	Manipulation of a degree of freedom. This class is compatible with a float value 
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	12/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class DegreeOfFreedom
		{
		protected:
			const Math::Interval<float> * m_constraint ;
			float * m_value ;
			DynamicNode * m_node ;

		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	DegreeOfFreedom::DegreeOfFreedom(DynamicNode * node,
			/// 	const Math::Interval<float> * constraint, float* value)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \param node 		The node providing this degree of freedom.
			/// \param constraint   The constraint.
			/// \param value		A pointer to the value associated with this degree of freedom.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			DegreeOfFreedom(DynamicNode * node, const Math::Interval<float> * constraint, float* value)
				: m_constraint(constraint), m_value(value), m_node(node)
			{
				assert(value!=NULL) ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	operator DegreeOfFreedom::float() const
			///
			/// \brief	float casting operator.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			////////////////////////////////////////////////////////////////////////////////////////////////////
			operator float() const
			{
				return *m_value ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const Math::Interval<float> & DegreeOfFreedom::constraint() const
			///
			/// \brief	Gets the constraint.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \return	.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const Math::Interval<float> & constraint() const
			{
				return *m_constraint ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	DynamicNode * DegreeOfFreedom::node() const
			///
			/// \brief	Gets the node associated with the DOF.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	17/02/2016
			///
			/// \return	null if it fails, else.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			DynamicNode * node() const
			{
				return m_node ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	DegreeOfFreedom & DegreeOfFreedom::operator= (float value);
			///
			/// \brief	Assignment operator. This operator ensures the constraints i.e. if the value is outside
			/// 		the authorized interval, it is rounded to the nearest valid value.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \param	value	The value.
			///
			/// \return	A shallow copy of this object.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			DegreeOfFreedom & operator = (float value)
			{
				(*m_value) = value ;
				m_node->update() ;
				return *this ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	Node
		///
		/// \brief	Super class of Nodes.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class Node
		{
		private:
			Node(const Node &) {}
			Node & operator=(const Node &) { return *this ; }

		protected:
			/// \brief	The father node.
			Node * m_father ;
			/// \brief	The sons nodes.
			::std::vector<Node*> m_sons ;
			/// \brief	The degrees of freedom.
			::std::vector<DegreeOfFreedom> m_degreesOfFreedom ;
			/// \brief	The transformation associated with this node.
			Math::Matrix4x4f m_transformation ;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Node::addSon(Node * son)
			///
			/// \brief	Adds a son node.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \param son	The new son node.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void addSon(Node * son)
			{
				assert(son!=NULL) ;
				assert(son->m_father==this) ;
				m_sons.push_back(son) ;
			}

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Node::Node(Node * father = NULL,
			/// 	const Math::Matrix4x4f & matrix = Math::Matrix4x4f::getIdentity())
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \param	father	(optional) [in,out] If non-null, the father node.
			/// \param	matrix	(optional) the transformation matrix associated with this node.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Node(Node * father = NULL, const Math::Matrix4x4f & matrix = Math::Matrix4x4f::getIdentity())
				: m_father(father), m_transformation(matrix)
			{
				if(m_father!=NULL)
				{
					m_father->addSon(this) ;
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const Math::Matrix4x4f & Node::getLocalTransformation() const
			///
			/// \brief	Gets the local transformation.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \return	The local transformation.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const Math::Matrix4x4f & getLocalTransformation() const
			{
				return m_transformation ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Math::Matrix4x4f Node::getGlobalTransformation() const
			///
			/// \brief	Gets the global transformation leading from the root to this node.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \return	The global transformation.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Math::Matrix4x4f getGlobalTransformation() const
			{
				Math::Matrix4x4f result = m_transformation ;
				Node * father = m_father ;
				while(father!=NULL)
				{
					result = father->m_transformation * result ;
					father = father->m_father ;
				}
				return result ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const ::std::vector<DegreeOfFreedom> & Node::getDOF() const
			///
			/// \brief	Gets the degrees of freedom of this node.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \return	The degree of freedom.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const ::std::vector<DegreeOfFreedom> & getDOF() const
			{
				return m_degreesOfFreedom ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const ::std::vector<Node*> Node::getSons() const
			///
			/// \brief	Gets the sons.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \return	The sons.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const ::std::vector<Node*> & getSons() const
			{
				return m_sons ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Node::collectDegreesOfFreedom(::std::vector<DegreeOfFreedom> & collected) const
			///
			/// \brief	Collects the degrees of freedom from this node to the root.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			///
			/// \param [in,out]	collected	The collected degrees of freedom.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void collectDegreesOfFreedom(::std::vector<DegreeOfFreedom> & collected) const
			{
				if(m_father!=NULL)
				{
					m_father->collectDegreesOfFreedom(collected) ;
				}
				::std::copy(m_degreesOfFreedom.begin(), m_degreesOfFreedom.end(), ::std::back_inserter(collected)) ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	virtual void Node::update()
			///
			/// \brief	Updates the current transformation. You should not call this function, update is 
			/// 		automatically triggered when a degree of freedom is modified.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	12/02/2016
			////////////////////////////////////////////////////////////////////////////////////////////////////
			virtual void update()
			{}

			virtual ~Node()
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	StaticNode
		///
		/// \brief	Super class of static nodes.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class StaticNode : public Node
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	StaticNode::StaticNode(Node * father = NULL,
			/// 	const Math::Matrix4x4f & matrix = Math::Matrix4x4f::getIdentity())
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	02/03/2016
			///
			/// \param	father	(optional) [in,out] If non-null, the father node.
			/// \param	matrix	(optional) the transformation matrix.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			StaticNode(Node * father = NULL, const Math::Matrix4x4f & matrix = Math::Matrix4x4f::getIdentity())
				: Node(father, matrix)
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	DynamicNode
		///
		/// \brief	Super class of dynamic nodes.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class DynamicNode : public Node
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	DynamicNode::DynamicNode(Node * father = NULL,
			/// 	const Math::Matrix4x4f & matrix = Math::Matrix4x4f::getIdentity())
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	02/03/2016
			///
			/// \param	father	(optional) [in,out] If non-null, the father node.
			/// \param	matrix	(optional) the transformation matrix.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			DynamicNode(Node * father = NULL, const Math::Matrix4x4f & matrix = Math::Matrix4x4f::getIdentity())
				: Node(father, matrix)
			{}
		};

	private:
		// We forbid the copy constructor and the affectation operator
		KinematicChain(KinematicChain const &) {}
		KinematicChain & operator=(const KinematicChain &) { return *this ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	StaticTranslation
		///
		/// \brief	Static translation.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class StaticTranslation : public StaticNode
		{
		public:
			StaticTranslation(Node * father, const Math::Vector3f & translation)
				: StaticNode(father, Math::Matrix4x4f::getTranslation(translation))
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	StaticEulerRotation
		///
		/// \brief	Static euler rotation.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class StaticEulerRotation : public StaticNode
		{
		public:
			StaticEulerRotation(Node * father, float angleX, float angleY, float angleZ)
				: StaticNode(father, Math::Matrix4x4f::getRotationX(angleX)*Math::Matrix4x4f::getRotationY(angleY)*Math::Matrix4x4f::getRotationZ(angleZ))
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	StaticScale
		///
		/// \brief	Static scale factor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class StaticScale : public StaticNode
		{
		public:
			StaticScale(Node * father, float sx, float sy, float sz)
				: StaticNode(father, Math::Matrix4x4f::getScale(sx, sy, sz))
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	EulerRotation
		///
		/// \brief	A dynamic Euler rotation. DOFs are the X,Y,Z angles.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class EulerRotation : public DynamicNode
		{
		protected:
			Math::Interval<float> m_ctrX, m_ctrY, m_ctrZ ;
			float m_angleX, m_angleY, m_angleZ ;

		public:
			EulerRotation(Node * father, const Math::Interval<float> & ctrX, float angleX, const Math::Interval<float> & ctrY, float angleY, const Math::Interval<float> & ctrZ, float angleZ)
				: DynamicNode(father), m_ctrX(ctrX), m_ctrY(ctrY), m_ctrZ(ctrZ), m_angleX(angleX), m_angleY(angleY), m_angleZ(angleZ)
			{
				assert(m_ctrX.contains(m_angleX)) ;
				assert(m_ctrY.contains(m_angleY)) ;
				assert(m_ctrZ.contains(m_angleZ)) ;
				update() ;
				m_degreesOfFreedom.push_back(DegreeOfFreedom(this, &m_ctrX, &m_angleX)) ;
				m_degreesOfFreedom.push_back(DegreeOfFreedom(this, &m_ctrY, &m_angleY)) ;
				m_degreesOfFreedom.push_back(DegreeOfFreedom(this, &m_ctrZ, &m_angleZ)) ;
			}

			virtual void update()
			{
				m_angleX = m_ctrX.clamp(m_angleX) ;
				m_angleY = m_ctrY.clamp(m_angleY) ;
				m_angleZ = m_ctrZ.clamp(m_angleZ) ;
				m_transformation = Math::Matrix4x4f::getRotationX(m_angleX)*Math::Matrix4x4f::getRotationY(m_angleY)*Math::Matrix4x4f::getRotationZ(m_angleZ) ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	Rotation
		///
		/// \brief	Axis angle rotation with 1 degree of freedom : the rotation angle
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	02/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class Rotation : public DynamicNode
		{
		protected:
			Math::Vector3f m_axis ;
			Math::Interval<float> m_ctrAngle ;
			float m_angle ;

		public:
			Rotation(Node * father, Math::Vector3f const & axis, const Math::Interval<float> & ctrAngle, float angle)
				: DynamicNode(father), m_axis(axis), m_ctrAngle(ctrAngle), m_angle(angle)
			{
				assert(m_ctrAngle.contains(m_angle)) ;
				update() ;
				m_degreesOfFreedom.push_back(DegreeOfFreedom(this, &m_ctrAngle, &m_angle)) ;
			}

			virtual void update()
			{
				m_angle = m_ctrAngle.clamp(m_angle) ;
				m_transformation = Math::Matrix4x4f::getRotation(m_axis, m_angle) ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	Translation
		///
		/// \brief	A dynamic translation. The DOF is the norm of the translation vector.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class Translation : public DynamicNode
		{
		protected:
			Math::Interval<float> m_ctrNorm ;
			float m_norm ;
			Math::Vector3f m_normalizedVector ;

		public:
			Translation(Node * father, Math::Interval<float> const & ctrNorm, Math::Vector3f const & translation)
				: DynamicNode(father), m_ctrNorm(ctrNorm), m_norm(translation.norm()), m_normalizedVector(translation.normalized())
			{
				assert(m_ctrNorm.contains(m_norm)) ;
				update() ;
				m_degreesOfFreedom.push_back(DegreeOfFreedom(this, &m_ctrNorm, &m_norm)) ;
			}

			virtual void update()
			{
				m_norm = m_ctrNorm.clamp(m_norm) ;
				m_transformation = Math::Matrix4x4f::getTranslation(m_normalizedVector*m_norm) ;
			}
		};

	protected:
		/// The root of the chain
		Node m_root ;

	private:
		void destroySons(Node * node)
		{
			for(auto it = node->getSons().begin(), end = node->getSons().end() ; it!=end ; ++it)
			{
				destroySons(*it) ;
				delete *it ;
			}
		}

		void destroy()
		{
			destroySons(&m_root) ;
		}

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	KinematicChain::KinematicChain()
		///
		/// \brief	Default constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		KinematicChain()
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Node * KinematicChain::getRoot()
		///
		/// \brief	Gets the root node.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		///
		/// \return	null if it fails, else the root.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Node * getRoot()
		{
			return &m_root ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	DynamicNode * KinematicChain::addDynamicTranslation(Node * father,
		/// 	Math::Interval<float> const & normConstraint, Math::Vector3f const & translation)
		///
		/// \brief	Adds a dynamic translation in the hierarchy. In this translation, only the norm of the vector
		/// 		is a DOF.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		///
		/// \param [in,out]	father	The father node (must not be null).
		/// \param	normConstraint	The norm constraint.
		/// \param	translation   	The translation.
		///
		/// \return	A pointer to the created node.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		DynamicNode * addDynamicTranslation(Node * father, Math::Interval<float> const & normConstraint, Math::Vector3f const & translation)
		{
			assert(father!=NULL) ;
			DynamicNode * result = new Translation(father, normConstraint, translation) ;
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	DynamicNode * KinematicChain::addDynamicEulerRotation(Node * father,
		/// 	const Math::Interval<float> & ctrX, float angleX, const Math::Interval<float> & ctrY,
		/// 	float angleY, const Math::Interval<float> & ctrZ, float angleZ)
		///
		/// \brief	Adds a dynamic euler rotation. DOFs are the X, Y and Z angles.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		///
		/// \param [in,out]	father	The father node (must not be null).
		/// \param	ctrX		  	The constraint on the X angle.
		/// \param	angleX		  	The angle x coordinate.
		/// \param	ctrY		  	The constraint on the Y angle.
		/// \param	angleY		  	The angle y coordinate.
		/// \param	ctrZ		  	The constraint on the Z angle.
		/// \param	angleZ		  	The angle z coordinate.
		///
		/// \return	The created node.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		DynamicNode * addDynamicEulerRotation(Node * father, const Math::Interval<float> & ctrX, float angleX, const Math::Interval<float> & ctrY, float angleY, const Math::Interval<float> & ctrZ, float angleZ)
		{
			assert(father!=NULL) ;
			DynamicNode * result = new EulerRotation(father, ctrX, angleX, ctrY, angleY, ctrZ, angleZ) ;
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	DynamicNode * KinematicChain::addDynamicRotation(Node * father, Math::Vector3f const & axis,
		/// 	const Math::Interval<float> & ctrAngle, float angle)
		///
		/// \brief	Adds a dynamic rotation.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	02/03/2016
		///
		/// \param [in,out]	father	The father node, must not be null.
		/// \param	axis		  	The axis.
		/// \param	ctrAngle	  	The angle constraint.
		/// \param	angle		  	The angle.
		///
		/// \return	A pointer to the created node.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		DynamicNode * addDynamicRotation(Node * father, Math::Vector3f const & axis, const Math::Interval<float> & ctrAngle, float angle)
		{
			assert(father!=NULL) ;
			DynamicNode * result = new Rotation(father, axis, ctrAngle, angle) ;
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	StaticNode * KinematicChain::addStaticTranslation(Node * father,
		/// 	Math::Vector3f const & translation)
		///
		/// \brief	Adds a static translation.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		///
		/// \param  father	The father node.
		/// \param	translation   	The translation vector.
		///
		/// \return	A pointer to the created node.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		StaticNode * addStaticTranslation(Node * father, Math::Vector3f const & translation)
		{
			assert(father!=NULL) ;
			StaticNode * result = new StaticTranslation(father, translation) ;
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	StaticNode * KinematicChain::addStaticEulerRotation(Node * father, float angleX, float angleY,
		/// 	float angleZ)
		///
		/// \brief	Adds a static euler rotation.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		///
		/// \param [in,out]	father	If non-null, the father.
		/// \param	angleX		  	The X rotation angle.
		/// \param	angleY		  	The Y rotation angle.
		/// \param	angleZ		  	The Z rotation angle.
		///
		/// \return	The newly created node.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		StaticNode * addStaticEulerRotation(Node * father, float angleX, float angleY, float angleZ)
		{
			assert(father!=NULL) ;
			StaticNode * result = new StaticEulerRotation(father, angleX, angleY, angleZ) ; 
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Vector3f KinematicChain::derivate(Node * extremity, const Math::Vector3f & offset,
		/// 	DegreeOfFreedom & dof, float epsilon) const
		///
		/// \brief	Derivates the position of the extremity of the chain with offset offset.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	11/02/2016
		///
		/// \param extremity	The extremity.
		/// \param offset		The offset.
		/// \param dof		 	The degree of freedom.
		/// \param epsilon		The epsilon used for derivation.
		///
		/// \return	The derivate.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Vector3f derivate(Node * extremity, const Math::Vector3f & offset, DegreeOfFreedom & dof, float epsilon) const
		{
			float referenceValue = dof ; 
			float realEpsilon = 0.0f ;
			dof = referenceValue+epsilon ;
			realEpsilon += dof-referenceValue ;
			Math::Vector3f transformPlus = extremity->getGlobalTransformation()*offset ;
			dof = referenceValue-epsilon ;
			realEpsilon += referenceValue-dof ;
			Math::Vector3f transformMinus = extremity->getGlobalTransformation()*offset ;
			dof = referenceValue ;
			return (transformPlus-transformMinus)/(realEpsilon) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	KinematicChain::~KinematicChain()
		///
		/// \brief	Destructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	12/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		~KinematicChain()
		{
			destroy() ;
		}
	};

}

#endif