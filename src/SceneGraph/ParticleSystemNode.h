#ifndef _SceneGraph_ParticleSystemNode_H
#define _SceneGraph_ParticleSystemNode_H

#include <SceneGraph/PointRenderer.h>
#include <Animation/ParticleSystem.h>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	ParticleSystemNode
	///
	/// \brief	Node displaying a particle system. 
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	04/04/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class ParticleSystemNode : public SceneGraph::PointRenderer
	{
	protected:
		Animation::ParticleSystem * m_particleSystem ;

	public:
		ParticleSystemNode(Animation::ParticleSystem * particleSystem, float particleSize=1.0f)
			: SceneGraph::PointRenderer(new HelperGl::Buffer<Math::Vector3f>(particleSystem->budget(), HelperGl::Buffer<Math::Vector3f>::ArrayBuffer), particleSize,
			new HelperGl::Buffer<HelperGl::Color>(particleSystem->budget(), HelperGl::Buffer<HelperGl::Color>::ArrayBuffer)
			),
			m_particleSystem(particleSystem)
		{}

		virtual void draw()
		{
			setPointCount(m_particleSystem->getParticles().size()) ;
			//m_particleSystem->update(0.001) ;
			{
				auto itBuffer = m_positionBuffer->begin() ;
				auto itColor = m_colorBuffer->begin() ;
				for(auto it=m_particleSystem->getParticles().begin(), end=m_particleSystem->getParticles().end() ; it!=end ; ++it)
				{
					*itBuffer = it->m_position ;	
					*itColor = it->m_color ;
					++itBuffer ;
					++itColor; 
				}
				m_positionBuffer->updateBuffer(0, m_particleSystem->getParticles().size()) ;
				m_colorBuffer->updateBuffer(0, m_particleSystem->getParticles().size()) ;
			}
			SceneGraph::PointRenderer::draw() ;

		}
	};
}


#endif