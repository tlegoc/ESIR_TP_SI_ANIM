#include <HelperGl/Mesh.h>

namespace HelperGl
{
	void Mesh::computeVertexNormal()
	{
		::std::vector<Math::Vector3f> vertexNormalAccumulator(m_vertices.size(), Math::Vector3f(0.0f));
		::std::vector<int> vertexNormalCount(m_vertices.size(), 0);

		for(size_t cpt=0 ; cpt<m_faces.size() ; ++cpt)
		{
			const Face & face = m_faces[cpt] ;
			for(int i=0 ; i<3 ; ++i)
			{
				vertexNormalAccumulator[face.m_verticesIndexes[i]] += face.m_normal ;
				vertexNormalCount[face.m_verticesIndexes[i]] += 1 ;
			}
		}

		for(size_t cpt=0 ; cpt<vertexNormalAccumulator.size() ; ++cpt)
		{
			if(vertexNormalCount[cpt]!=0)
			{
				vertexNormalAccumulator[cpt] /= (GLfloat)vertexNormalCount[cpt] ;
				vertexNormalAccumulator[cpt] = vertexNormalAccumulator[cpt].normalized();
			}
		}
		m_verticesNormals = vertexNormalAccumulator ;
	}

	void Mesh::clean()
	{
		// 1 - We compute used vertices and texture coordinates
		::std::set<size_t> usedVertices ;
		::std::set<size_t> usedTextureCoordinates ;
		for(auto it=m_faces.begin() ; it!=m_faces.end() ; ++it)
		{
			::std::copy(it->m_verticesIndexes, it->m_verticesIndexes+3, ::std::inserter(usedVertices, usedVertices.begin()));
			::std::copy(it->m_textureIndexes, it->m_textureIndexes+3, ::std::inserter(usedTextureCoordinates, usedTextureCoordinates.begin())) ;
		}
		// 2 - We recompute the vertex table 
		::std::vector<Math::Vector3f> newVertices ;
		::std::vector<Math::Vector3f> newVerticesNormals ;
		::std::vector<size_t> vertexTranslationTable(m_vertices.size()) ;
		for(auto it=usedVertices.begin() ; it!=usedVertices.end() ; ++it)
		{
			size_t vertexIndex = *it ;
			newVertices.push_back(m_vertices[vertexIndex]) ;
			if(m_verticesNormals.size()!=0)
			{
				newVerticesNormals.push_back(m_verticesNormals[vertexIndex]) ;
			}
			vertexTranslationTable[vertexIndex] = newVertices.size()-1 ;
		}
		m_vertices = newVertices ;
		m_verticesNormals = newVerticesNormals ;
		// 3 - We recompute the texture coordinates table
		::std::vector<Math::Vector2f> newTextureCoordinates ;
		::std::vector<size_t> textureTranslationTable(m_textureCoordinates.size()) ;
		for(auto it=usedTextureCoordinates.begin() ; it!=usedTextureCoordinates.end() ; ++it)
		{
			size_t textureIndex = *it ;
			newTextureCoordinates.push_back(m_textureCoordinates[textureIndex]) ;
			textureTranslationTable[textureIndex] = newTextureCoordinates.size()-1 ;
		}
		m_textureCoordinates = newTextureCoordinates ;
		// 4 - We update the triangles
		for(auto it=m_faces.begin() ; it!=m_faces.end() ; ++it)
		{
			::std::transform(it->m_verticesIndexes, it->m_verticesIndexes+3, it->m_verticesIndexes, [&vertexTranslationTable](size_t index) -> size_t { return vertexTranslationTable[index] ; } ) ;
			::std::transform(it->m_textureIndexes, it->m_textureIndexes+3, it->m_textureIndexes, [&textureTranslationTable](size_t index) -> size_t { return textureTranslationTable[index] ; } ) ;
		}
	}

}