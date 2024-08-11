#ifndef _HelperGL_LoaderOgre3D_H
#define _HelperGL_LoaderOgre3D_H

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <HelperGl/Mesh.h>
#include <deque>

// If we compile under windows, we disable a strange warning about exceptions (nothing important).
#ifdef _MSC_BUILD
#pragma warning( disable : 4290 )
#endif

namespace HelperGl
{
	class LoaderOgre3D
	{
		//Mesh * loadMesh(rapidxml::xml_node * subMeshNode)
		//{
		//	Mesh * current = new Mesh ;
		//	::std::vector<Math::Vector3f> vertices ;
		//	::std::vector<Math::Vector3f> normals ;
		//	::std::vector<Math::Vector2f> textureCoordinates ;
		//	
		//}

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	Skeleton
		///
		/// \brief	A skeleton described in a .skeleton file from Ogre.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class Skeleton
		{
		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \struct	Bone
			///
			/// \brief	A bone.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	07/04/2016
			////////////////////////////////////////////////////////////////////////////////////////////////////
			struct Bone
			{
				/// \brief	The identifier.
				::std::string m_id ;
				/// \brief	The name.
				::std::string m_name ;
				/// \brief	The position.
				Math::Vector3f m_position ;
				/// \brief	The rotation axis.
				Math::Vector3f m_rotationAxis ;
				/// \brief	The rotation angle.
				float m_rotationAngle ;
				/// \brief	The parent.
				Bone * m_parent ;
				/// \brief	The sons.
				::std::vector<Bone*> m_sons ;

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \fn	Bone()
				///
				/// \brief	Default constructor.
				///
				/// \author	F. Lamarche, Université de Rennes 1
				/// \date	07/04/2016
				////////////////////////////////////////////////////////////////////////////////////////////////////
				Bone()
					: m_position(0.0f), m_rotationAxis(0.0f), m_rotationAngle(0.0f), m_parent(NULL)
				{}
			};

		protected:
			/// \brief	The bones.
			::std::deque<Bone> m_bones ;
			/// \brief	The name to bone map.
			::std::map<::std::string, Bone*> m_nameToBone ;
			/// \brief	The identifier to bone map.
			::std::map<::std::string, Bone*> m_idToBone ;
			/// \brief	Zero-based index of the identifier to bone.
			::std::map<::std::string, unsigned int> m_idToBoneIndex ;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Skeleton::addBone(const Bone & bone)
			///
			/// \brief	Adds a bone.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	08/04/2016
			///
			/// \param	bone	The bone.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void addBone(const Bone & bone) 
			{
				m_bones.push_back(bone) ;
				m_nameToBone[bone.m_name] = &m_bones.back() ;
				m_idToBone[bone.m_id] = &m_bones.back() ;
				m_idToBoneIndex[bone.m_id] = m_bones.size()-1 ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Skeleton::addParentChildRelation(const ::std::string & parent,
			/// 	const ::std::string & child)
			///
			/// \brief	Adds a parent child relation between bones.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	07/04/2016
			///
			/// \param	parent	The parent.
			/// \param	child 	The child.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void addParentChildRelation(const ::std::string & parent, const ::std::string & child)
			{
				Bone * bParent = m_nameToBone[parent] ;
				Bone * bChild = m_nameToBone[child] ;
				bParent->m_sons.push_back(bChild) ;
				bChild->m_parent = bParent ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const Bone * Skeleton::findRoot() const
			///
			/// \brief	Searches for the first root bone.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	07/04/2016
			///
			/// \return	null if it fails, else the found root.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const Bone * findRoot() const
			{
				for(auto it=m_bones.begin(), end=m_bones.end() ; it!=end ; ++it)
				{
					if(it->m_parent==NULL)
					{
						return &(*it) ;
					}
				}
				return nullptr;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	int Skeleton::nbRoot() const
			///
			/// \brief	Gets the number root nodes (should be one but...).
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	08/04/2016
			///
			/// \return	.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			int nbRoot() const
			{
				int result = 0 ;
				for(auto it=m_bones.begin(), end=m_bones.end() ; it!=end ; ++it)
				{
					if(it->m_parent==NULL)
					{
						++result ;
					}
				}
				return result ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const Bone * Skeleton::findBoneByName(::std::string const & name)
			///
			/// \brief	Searches for a bone by name.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	07/04/2016
			///
			/// \param	name	The name.
			///
			/// \return	null if it fails, else the found bone by name.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const Bone * findBoneByName(::std::string const & name)
			{
				return m_nameToBone[name] ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const Bone * Skeleton::findBoneById(::std::string const & id)
			///
			/// \brief	Searches for a bone by identifier.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	07/04/2016
			///
			/// \param	id	The identifier.
			///
			/// \return	null if it fails, else the found bone by identifier.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const Bone * findBoneById(::std::string const & id)
			{
				return m_idToBone[id] ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	unsigned int Skeleton::findBoneIndexById(::std::string const & id)
			///
			/// \brief	Searches for a bone index by identifier.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	08/04/2016
			///
			/// \param	id	The identifier.
			///
			/// \return	The index of the bone.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			unsigned int findBoneIndexById(::std::string const & id)
			{
				return m_idToBoneIndex[id] ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	LoaderException
		///
		/// \brief	Exception for signalling loader errors.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class LoaderException : public std::exception
		{
			::std::string m_msg ;
		public:
			LoaderException(const ::std::string & msg)
			{}

			virtual const char * what() const
			{
				return m_msg.c_str() ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	MissingNodeException
		///
		/// \brief	Exception for signaling missing node errors.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class MissingNodeException : public LoaderException
		{
		public:
			MissingNodeException(const ::std::string & msg)
				: LoaderException(msg)
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	MissingAttributeException
		///
		/// \brief	Exception for signaling missing attribute errors.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class MissingAttributeException : public LoaderException
		{
		public:
			MissingAttributeException(const ::std::string & msg)
				: LoaderException(msg)
			{}
		};

	protected:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void LoaderOgre3D::notNull(rapidxml::xml_node<> * node,
		/// 	const ::std::string & msg) throw (MissingNodeException)
		///
		/// \brief	If node is null, this methods throws an exception :).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		///
		/// \exception	MissingNodeException	Thrown when a Missing Node error condition occurs.
		///
		/// \param [in,out]	node	If non-null, the node.
		/// \param	msg				The message.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void notNull(rapidxml::xml_node<> * node, const ::std::string & msg) throw (MissingNodeException)
		{
			if(node==NULL) { throw MissingNodeException(msg) ; }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void LoaderOgre3D::notNull(rapidxml::xml_attribute<> * node,
		/// 	const ::std::string & msg) throw (MissingAttributeException)
		///
		/// \brief	If node is null, this methods throws an exception :).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		///
		/// \exception	MissingAttributeException	Thrown when a Missing Attribute error condition
		/// 										occurs.
		///
		/// \param [in,out]	node	If non-null, the node.
		/// \param	msg				The message.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void notNull(rapidxml::xml_attribute<> * node, const ::std::string & msg) throw (MissingAttributeException)
		{
			if(node==NULL) { throw MissingAttributeException(msg) ; }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Vector3f LoaderOgre3D::parseVector3f(rapidxml::xml_node<> * node) throw (MissingAttributeException)
		///
		/// \brief	Parse a Vector3f in the WML node (search for x,y and z attributes).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		///
		/// \exception	MissingAttributeException	Thrown when a Missing Attribute error condition
		/// 										occurs.
		///
		/// \param [in,out]	node	If non-null, the node.
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Vector3f parseVector3f(rapidxml::xml_node<> * node) throw (MissingAttributeException)
		{
			rapidxml::xml_attribute<> * x = node->first_attribute("x") ;
			notNull(x, "Missing x attribute!") ;
			rapidxml::xml_attribute<> * y = node->first_attribute("y") ;
			notNull(y, "Missing y attribute!") ;
			rapidxml::xml_attribute<> * z = node->first_attribute("z") ;
			notNull(z, "Missing z attribute!") ;
			return Math::makeVector(atof(x->value()), atof(y->value()), atof(z->value())) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Vector2f LoaderOgre3D::parseVector2fuv(rapidxml::xml_node<> * node)
		///
		/// \brief	Parse a texture coordinate vector 2f.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	08/04/2016
		///
		/// \param [in,out]	node	If non-null, the node.
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Vector2f parseVector2fuv(rapidxml::xml_node<> * node)
		{
			rapidxml::xml_attribute<> * u = node->first_attribute("u") ;
			notNull(u, "Missing u attribute!") ;
			rapidxml::xml_attribute<> * v = node->first_attribute("v") ;
			notNull(v, "Missing v attribute!") ;
			return Math::makeVector(atof(u->value()), atof(v->value())) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	float LoaderOgre3D::parseAngle(rapidxml::xml_node<> * node) throw (MissingAttributeException)
		///
		/// \brief	Parse an angle (search for angle attribute).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		///
		/// \exception	MissingAttributeException	Thrown when a Missing Attribute error condition
		/// 										occurs.
		///
		/// \param [in,out]	node	If non-null, the node.
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float parseAngle(rapidxml::xml_node<> * node) throw (MissingAttributeException)
		{
			rapidxml::xml_attribute<> * angle = node->first_attribute("angle") ;
			notNull(angle, "Missing angle attribute!") ;
			return (float)atof(angle->value()) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void LoaderOgre3D::parseBonesHierarchy( rapidxml::xml_node<> * node, Skeleton * skeleton )
		///
		/// \brief	Parse bones hierarchy.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	08/04/2016
		///
		/// \param [in,out]	node		If non-null, the node.
		/// \param [in,out]	skeleton	If non-null, the skeleton.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void parseBonesHierarchy( rapidxml::xml_node<> * node, Skeleton * skeleton ) 
		{
			rapidxml::xml_node<> * boneHierarchy = node->first_node("bonehierarchy") ;
			notNull(boneHierarchy, "Missing <bonehierarchy> node!") ;
			rapidxml::xml_node<> * boneRelationIterator = boneHierarchy->first_node("boneparent") ;
			while(boneRelationIterator!=NULL)
			{
				rapidxml::xml_attribute<> * boneAttribute = boneRelationIterator->first_attribute("bone") ;
				notNull(boneAttribute, "Missing bone attribute in <boneparent> node!") ;
				::std::string bone = boneAttribute->value() ;
				rapidxml::xml_attribute<> * parentAttribute = boneRelationIterator->first_attribute("parent") ;
				notNull(parentAttribute, "Missing parent attribute in <boneparent> node!") ;
				::std::string parent = parentAttribute->value() ;
				skeleton->addParentChildRelation(parent, bone) ;
				boneRelationIterator = boneRelationIterator->next_sibling("boneparent"); 
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void LoaderOgre3D::parseBones( rapidxml::xml_node<> * node, Skeleton * skeleton )
		///
		/// \brief	Parse bones.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	08/04/2016
		///
		/// \param [in,out]	node		If non-null, the node.
		/// \param [in,out]	skeleton	If non-null, the skeleton.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void parseBones( rapidxml::xml_node<> * node, Skeleton * skeleton ) 
		{
			rapidxml::xml_node<> * bones = node->first_node("bones") ;
			notNull(bones, "Missing <bones> node") ;
			rapidxml::xml_node<> * boneIterator = bones->first_node("bone") ;
			while(boneIterator!=NULL)
			{
				Skeleton::Bone bone;
				rapidxml::xml_attribute<> * id = boneIterator->first_attribute("id") ;
				notNull(id, "Missing id attribute in <bone> node") ;
				bone.m_id = id->value() ; 
				rapidxml::xml_attribute<> * name = boneIterator->first_attribute("name") ;
				notNull(name, "Missing name attribute in <bone> node") ;
				bone.m_name = name->value() ; 
				rapidxml::xml_node<> * position = boneIterator->first_node("position") ;
				notNull(position, "Missing <position> node in <bone> node") ;
				bone.m_position = parseVector3f(position) ;
				rapidxml::xml_node<> * rotation = boneIterator->first_node("rotation") ;
				notNull(position, "Missing <rotation> node in <bone> node") ;
				bone.m_rotationAngle = parseAngle(rotation) ;
				rapidxml::xml_node<> * axis = rotation->first_node("axis") ;
				notNull(axis, "Missing <axis> node in <rotation> node") ;
				bone.m_rotationAxis = parseVector3f(axis) ;
				skeleton->addBone(bone) ;
				boneIterator = boneIterator->next_sibling("bone") ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Skeleton * LoaderOgre3D::parseSkeleton(rapidxml::xml_node<> * node) throw (MissingAttributeException,
		/// 	MissingNodeException)
		///
		/// \brief	Parse a skeleton node.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		///
		/// \exception	MissingAttributeException	Thrown when a Missing Attribute error condition
		/// 										occurs.
		/// \exception	MissingNodeException	 	Thrown when a Missing Node error condition occurs.
		///
		/// \param [in,out]	node	If non-null, the node.
		///
		/// \return	null if it fails, else.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Skeleton * parseSkeleton(rapidxml::xml_node<> * node) throw (MissingAttributeException, MissingNodeException)
		{
			Skeleton * skeleton = new Skeleton ;
			// Loads bones
			parseBones(node, skeleton);
			// Loads bones hierarchy
			parseBonesHierarchy(node, skeleton);
			return skeleton ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Vector3f LoaderOgre3D::parseVertex(rapidxml::xml_node<> * node,
		/// 	const ::std::string & subNodeName)
		///
		/// \brief	Parse a sub node of a vertex node (position, normal -> provided by subNodeName).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	08/04/2016
		///
		/// \param [in,out]	node	If non-null, the node.
		/// \param	subNodeName 	Name of the sub node.
		///
		/// \return	The 3D vector.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Vector3f parseVertex(rapidxml::xml_node<> * node, const ::std::string & subNodeName)
		{
			assert(::std::string(node->value())=="vertex") ;
			Math::Vector3f result;
			rapidxml::xml_node<> * subNode = node->first_node(subNodeName.c_str()) ;
			notNull(subNode, ::std::string("Missing ")+subNodeName+" node");
			return parseVector3f(subNode) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	::std::vector<Math::Vector3f> LoaderOgre3D::parseVertexBuffer(rapidxml::xml_node<> * node,
		/// 	const ::std::string & subNodeName)
		///
		/// \brief	Parse a vertex buffer and extract the position or normal of each vertex
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	08/04/2016
		///
		/// \param [in,out]	node	If non-null, the node.
		/// \param	subNodeName 	Name of the sub node (position or normal).
		////////////////////////////////////////////////////////////////////////////////////////////////////
		::std::vector<Math::Vector3f> parseVertexBuffer(rapidxml::xml_node<> * node, const ::std::string & subNodeName)
		{
			assert(::std::string(node->value())=="vertexbuffer") ;
			::std::vector<Math::Vector3f> buffer ;
			rapidxml::xml_node<> * vertex_iterator = node->first_node("vertex") ;
			while(vertex_iterator!=NULL)
			{
				buffer.push_back(parseVertex(vertex_iterator, subNodeName)) ;
				vertex_iterator = vertex_iterator->next_sibling("vertex");
			}
			return buffer ;
		}

		::std::vector<Math::Vector2f> parseTextureCoordinatesBuffer(rapidxml::xml_node<> * node)
		{
			assert(::std::string(node->value())=="vertexbuffer") ;
			::std::vector<Math::Vector2f> buffer ;
			rapidxml::xml_node<> * vertex_iterator = node->first_node("vertex") ;
			while(vertex_iterator!=NULL)
			{
				rapidxml::xml_node<> * textureCoordinates = vertex_iterator->first_node("texcoord") ;
				notNull(textureCoordinates, "Missing node <textcoord> in <vertex>") ;
				buffer.push_back(parseVector2fuv(textureCoordinates)) ;
				vertex_iterator = vertex_iterator->next_sibling("vertex");
			}
			return buffer ;
		}

		void parseVertexBuffer(rapidxml::xml_node<> * node, 
							   bool & hasPositions, ::std::vector<Math::Vector3f> & position, 
							   bool & hasNormals, ::std::vector<Math::Vector3f> & normals,
							   bool & hasTextureCoordinates, ::std::vector<Math::Vector2f> & textureCoodinates)
		{
			assert(::std::string(node->value())=="vertexbuffer") ;
			hasPositions = false ;
			hasNormals = false ;
			hasTextureCoordinates = false ;
			rapidxml::xml_attribute<> * positionAttrib = node->first_attribute("positions") ;
			if(positionAttrib!= NULL && ::std::string(positionAttrib->value())=="true") 
			{ 
				hasPositions = true ;
				position = parseVertexBuffer(node, "position") ;
			}
			rapidxml::xml_attribute<> * normalAttrib = node->first_attribute("normals") ;
			if(normalAttrib!=NULL && ::std::string(normalAttrib->value())=="true")
			{
				hasNormals = true ;
				normals = parseVertexBuffer(node, "normal") ;
			}
			rapidxml::xml_attribute<> * textureAttrib = node->first_attribute("texture_coords") ;
			if(textureAttrib!=NULL)
			{
				hasTextureCoordinates = true ;
				parseTextureCoordinatesBuffer(node) ;
			}
		}

		struct Mesh
		{
			struct Face
			{
				unsigned int m_indexes[3] ;
			};

			::std::vector<Math::Vector3f> m_positions ;
			::std::vector<Math::Vector3f> m_normals ;
			::std::vector<Math::Vector2f> m_textureCoodinates ;
			::std::vector<Face> m_faces ;
		};

		void parseGeometry(rapidxml::xml_node<> * node, Mesh & mesh)
		{
			assert(::std::string(node->value())=="geometry") ;
			rapidxml::xml_node<> * bufferIterator = node->first_node("vertexbuffer") ;
			bool hasPositions ;
			bool hasNormals ; 
			bool hasTextureCoordinates ;

			while(bufferIterator!=NULL)
			{
				parseVertexBuffer(node, hasPositions, mesh.m_positions, hasNormals, mesh.m_normals, hasTextureCoordinates, mesh.m_textureCoodinates) ;
			}
		}

		Mesh::Face parseFace(rapidxml::xml_node<> * node)
		{
			assert(::std::string(node->value())=="face") ;
			Mesh::Face face ;
			rapidxml::xml_attribute<> * v1 = node->first_attribute("v1") ;
			notNull(v1, "Missing v1 attribute!") ;
			rapidxml::xml_attribute<> * v2 = node->first_attribute("v2") ;
			notNull(v2, "Missing v2 attribute!") ;
			rapidxml::xml_attribute<> * v3 = node->first_attribute("v3") ;
			notNull(v2, "Missing v3 attribute!") ;

			face.m_indexes[0] = atof(v1->value()) ;
			face.m_indexes[1] = atof(v2->value()) ;
			face.m_indexes[2] = atof(v3->value()) ;

			return face ;
		}

		void parseFaces(rapidxml::xml_node<> * node, Mesh & mesh)
		{
			assert(::std::string(node->value())=="faces") ;
			rapidxml::xml_node<> * faceIterator = node->first_node("face") ;
			while(faceIterator!=NULL)
			{
				mesh.m_faces.push_back(parseFace(faceIterator)) ;
				faceIterator = faceIterator->next_sibling("face") ;
			}
		}

		void parseBoneAssignment(rapidxml::xml_node<> * node)
		{
			assert(::std::string(node->value())=="boneassignments") ;
		}

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Skeleton * LoaderOgre3D::parseSkeleton(const ::std::string & filename)
		///
		/// \brief	Loads a skeleton file.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/04/2016
		///
		/// \param	filename	Filename of the file.
		///
		/// \return	null if it fails, else the skeleton.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Skeleton * loadSkeleton(const ::std::string & filename) throw (MissingAttributeException, MissingNodeException)
		{
			rapidxml::file<> xmlFile(filename.c_str()); 
			rapidxml::xml_document<> doc;
			doc.parse<0>(xmlFile.data());
			return parseSkeleton(doc.first_node("skeleton")) ;
		}
	};
}

#endif