#if defined(ZENIC_WIN32)
inline void DrawingEnvironment::setRenderState( uint state, u32 value )
{
	ZENIC_ASSERT( state < RenderStates );

	if( value == m_renderStates[state] )
		return;
	m_renderStates[state] = value;
	m_device->SetRenderState( static_cast<D3DRENDERSTATETYPE>(state), value );
}
#endif

inline void DrawingEnvironment::setFloatRenderState( uint state, f32 value )
{
	setRenderState( state, *reinterpret_cast<u32*>(&value) );
}

inline u32 DrawingEnvironment::renderState( uint state )
{
	ZENIC_ASSERT( state < RenderStates );
	return m_renderStates[state];
}

#if defined(ZENIC_WIN32)
inline void DrawingEnvironment::setTextureStageState( uint stage, uint state, u32 value )
{
	ZENIC_ASSERT( stage < TextureStages );
	ZENIC_ASSERT( state < TextureStageStates );

	if( value ==  m_textureStages[stage][state] )
		return;
	m_textureStages[stage][state] = value;
	m_device->SetTextureStageState( stage, static_cast<D3DTEXTURESTAGESTATETYPE>(state), value );
}
#endif

inline u32 DrawingEnvironment::textureStageState( uint stage, uint state )
{
	ZENIC_ASSERT( stage < TextureStages );
	ZENIC_ASSERT( state < TextureStageStates );

	return m_textureStages[stage][state];
}

#if defined(ZENIC_WIN32)
inline void DrawingEnvironment::setSamplerState( uint sampler, uint state, u32 value )
{
	ZENIC_ASSERT( sampler < Samplers );
	ZENIC_ASSERT( state < SamplerStates );

	if( value == m_samplers[sampler][state] )
		return;
	m_samplers[sampler][state] = value;
	m_device->SetSamplerState( sampler, static_cast<D3DSAMPLERSTATETYPE>(state), value );
}
#endif

inline u32 DrawingEnvironment::samplerState( uint sampler, uint state )
{
	ZENIC_ASSERT( sampler < Samplers );
	ZENIC_ASSERT( state < SamplerStates );

	return m_samplers[sampler][state];
}

inline RenderTarget* DrawingEnvironment::renderTarget( uint index )
{
	ZENIC_ASSERT( index < RenderTargets );
	return m_renderTargets[index];
}

inline Texture* DrawingEnvironment::texture( uint stage )
{
	ZENIC_ASSERT( stage < Textures );
	return m_textures[stage];
}
