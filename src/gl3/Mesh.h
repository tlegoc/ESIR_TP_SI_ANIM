#pragma once

#include <vector>
#include <optional>
#include <limits>
#include <glm/glm.hpp>
#include <gl3/BoundingBox.h>
#include <gl3/utils.h>
#include <gl3/VertexBufferObject.h>
#include <gl3/ElementBufferObject.h>

namespace gl3
{
	/// <summary>
	/// Mesh data
	/// </summary>
	class Mesh
	{
	private:
		std::optional<std::vector<glm::vec3>> m_vertices;
		std::optional<std::vector<glm::vec3>> m_normals;
		std::optional<std::vector<glm::vec3>> m_tangents;
		std::optional<std::vector<glm::vec3>> m_bitangents;
		std::optional<std::vector<glm::vec2>> m_textureCoordinates;
		std::optional<std::vector<GLuint>> m_indices;

		mutable std::optional<VertexBufferObject> m_verticesVbo;
		mutable std::optional<VertexBufferObject> m_normalsVbo;
		mutable std::optional<VertexBufferObject> m_tangentsVbo;
		mutable std::optional<VertexBufferObject> m_bitangentsVbo;
		mutable std::optional<VertexBufferObject> m_textureCoordinatesVbo;
		mutable std::optional<ElementBufferObject> m_indicesEbo;

		mutable std::optional<BoundingBox> m_boundingBox;

		/// <summary>
		/// Updates the extent.
		/// </summary>
		void updateBoundingBox() const
		{
			assert(m_vertices && "Mesh::updateBoundingBox called with no vertices in the mesh");
			if (m_boundingBox) { return; }
			m_boundingBox = BoundingBox();
			(*m_boundingBox).update((*m_vertices).begin(), (*m_vertices).end());
		}

		/// <summary>
		/// Creates the vbo if needed.
		/// </summary>
		/// <param name="vbo">The vbo.</param>
		/// <param name="data">The data.</param>
		template <typename Type>
		void createVbo(std::optional<VertexBufferObject> & vbo, const std::vector<Type> & data) const
		{
			if (vbo) { return; }
			vbo = VertexBufferObject(data);
		}

		/// <summary>
		/// Updates the vbo.
		/// </summary>
		/// <param name="vbo">The vbo.</param>
		/// <param name="data">The data.</param>
		template <typename Type>
		void updateVbo(std::optional<VertexBufferObject> & vbo, const std::vector<Type> & data) const
		{
			if (!vbo) { return; }
			(*vbo).update(data);
		}

		/// <summary>
		/// Creates the element buffer object.
		/// </summary>
		void createEbo() const
		{
			if (m_indicesEbo) { return; }
			m_indicesEbo = ElementBufferObject(*m_indices);
		}

	public:
		typedef enum {} DataType;

		/// <summary>
		/// Initializes en empty mesh.
		/// </summary>
		Mesh() {}

		/// <summary>
		/// Computes the extend of the geometry.
		/// </summary>
		/// <returns></returns>
		const BoundingBox & boundingBox() const
		{
			updateBoundingBox();
			return *m_boundingBox;
		}

		/// <summary>
		/// Sets the vertices.
		/// </summary>
		/// <param name="vertices">The vertices.</param>
		void setVertices(const std::vector<glm::vec3> & vertices) 
		{
			m_boundingBox.reset();
			m_verticesVbo.reset();
			m_vertices = vertices; 
		}

		/// <summary>
		/// Gets the vertices.
		/// </summary>
		/// <returns></returns>
		const std::optional<std::vector<glm::vec3>> & getVertices() const { return m_vertices; }

		/// <summary>
		/// Gets a pointer to the vertices vertex buffer object.
		/// </summary>
		/// <returns></returns>
		const VertexBufferObject * getVerticesVbo() const
		{
			assert(m_vertices && "Mesh::getVerticesVbo called with no vertices in the mesh");
			createVbo(m_verticesVbo, *m_vertices);
			return &(*m_verticesVbo);
		}

		/// <summary>
		/// Sets the normals.
		/// </summary>
		/// <param name="normals">The normals.</param>
		void setNormals(const std::vector<glm::vec3> & normals) 
		{
			m_normalsVbo.reset();
			m_normals = normals; 
		}

		/// <summary>
		/// Gets the normals.
		/// </summary>
		/// <returns></returns>
		const std::optional<std::vector<glm::vec3>> & getNormals() const { return m_normals; }

		/// <summary>
		/// Gets the normals vertex buffer object.
		/// </summary>
		/// <returns></returns>
		const VertexBufferObject * getNormalsVbo() const
		{
			assert(m_normals && "Mesh::getNormalsVbo called with no normals in the mesh");
			createVbo(m_normalsVbo, *m_normals);
			return &(*m_normalsVbo);
		}

		/// <summary>
		/// Sets the tagents.
		/// </summary>
		/// <param name="tangents">The tangents.</param>
		void setTagents(const std::vector<glm::vec3> & tangents) 
		{
			m_tangentsVbo.reset();
			m_tangents = tangents; 
		}

		/// <summary>
		/// Gets the tangents.
		/// </summary>
		/// <returns></returns>
		const std::optional<std::vector<glm::vec3>> & getTangents() const { return m_tangents; }

		/// <summary>
		/// Gets the tangents vertex buffer object.
		/// </summary>
		/// <returns></returns>
		const VertexBufferObject * getTagentsVbo() const
		{
			assert(m_tangents && "Mesh::getTagentsVbo called with no tangents in the mesh");
			createVbo(m_normalsVbo, *m_normals);
			return &(*m_normalsVbo);
		}

		/// <summary>
		/// Sets the bitagents.
		/// </summary>
		/// <param name="bitangents">The bitangents.</param>
		void setBitagents(const std::vector<glm::vec3> & bitangents) 
		{
			m_tangentsVbo.reset();
			m_bitangents = bitangents; 
		}

		/// <summary>
		/// Gets the bitangents.
		/// </summary>
		/// <returns></returns>
		const std::optional<std::vector<glm::vec3>> & getBitangents() const { return m_bitangents; }

		/// <summary>
		/// Gets the bitangents vertex buffer object.
		/// </summary>
		/// <returns></returns>
		const VertexBufferObject * getBitangentsVbo() const
		{
			assert(m_bitangents && "Mesh::getBitangentsVbo called with no bitangents in the mesh");
			createVbo(m_bitangentsVbo, *m_bitangents);
			return &(*m_bitangentsVbo);
		}

		/// <summary>
		/// Sets the texture coodinates.
		/// </summary>
		/// <param name="textureCoordinates">The texture coordinates.</param>
		void setTextureCoodinates(const std::vector<glm::vec2> & textureCoordinates) 
		{
			m_textureCoordinatesVbo.reset();
			m_textureCoordinates = textureCoordinates; 
		}

		/// <summary>
		/// Gets the texture coordinates.
		/// </summary>
		/// <returns></returns>
		const std::optional<std::vector<glm::vec2>> & getTextureCoordinates() const { return m_textureCoordinates; }

		/// <summary>
		/// Gets the texture coordinates vertex buffer object.
		/// </summary>
		/// <returns></returns>
		const VertexBufferObject * getTextureCoordinatesVbo() const
		{
			assert(m_textureCoordinates && "Mesh::getTextureCoordinatesVbo called with no texture coordinates in the mesh");
			createVbo(m_textureCoordinatesVbo, *m_textureCoordinates);
			return &(*m_textureCoordinatesVbo);
		}

		/// <summary>
		/// Sets the indices.
		/// </summary>
		/// <param name="indices">The indices.</param>
		void setIndices(const std::vector<GLuint> & indices) 
		{
			m_indicesEbo.reset();
			m_indices = indices; 
		}

		/// <summary>
		/// Gets the indices.
		/// </summary>
		/// <returns></returns>
		const std::optional<std::vector<GLuint>> & getIndices() const { return m_indices; }

		/// <summary>
		/// Gets the indices element buffer object.
		/// </summary>
		/// <returns></returns>
		const ElementBufferObject * getIndicesEbo() const
		{
			assert(m_indices && "Mesh::getIndicesEbo called with no indices in the mesh");
			createEbo();
			return &(*m_indicesEbo);
		}

		/// <summary>
		/// Applies a transformation to all vertices and normals of this mesh.
		/// </summary>
		/// <param name="transform">The transform.</param>
		void applyTransform(const glm::mat4 & transform)
		{
			if (m_vertices)
			{
				gl3::applyTransform(transform, *m_vertices);
				updateVbo(m_verticesVbo, *m_vertices);
				m_boundingBox.reset();
			}
			if (m_normals)
			{
				glm::mat3 normalTransform = glm::transpose(inverse(glm::mat3(transform)));
				gl3::applyTransform(normalTransform, *m_normals);
				updateVbo(m_normalsVbo, *m_normals);
			}
		}

		/// <summary>
		/// Applies a transformation to all vertices and normals of this mesh.
		/// </summary>
		/// <param name="transform">The transform.</param>
		void applyTransform(const glm::mat4x3 & transform)
		{
			if (m_vertices)
			{
				gl3::applyTransform(transform, *m_vertices);
				updateVbo(m_verticesVbo, *m_vertices);
				m_boundingBox.reset();
			}
			if (m_normals)
			{
				glm::mat3 normalTransform = glm::transpose(inverse(glm::mat3(transform)));
				gl3::applyTransform(normalTransform, *m_normals);
				updateVbo(m_normalsVbo, *m_normals);
			}
		}

		/// <summary>
		/// Trasnforms the geometry such as its fits the provided bounding box.
		/// </summary>
		/// <param name="box">The box.</param>
		void fit(const BoundingBox & box)
		{
			BoundingBox thisBB = boundingBox();
			glm::mat4 transform = glm::translate(box.center())*glm::scale(box.extent()/boundingBox().extent())*glm::translate(-thisBB.center());
			applyTransform(transform);
		}
	};
}