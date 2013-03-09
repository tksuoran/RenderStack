#include "renderer/renderer.hpp"

#if 0
#define DEBUG_RENDERER

private void InitializeAttributeMappings()
{
   var attributeMappings = AttributeMappings.Global;
   attributeMappings.Clear();
   attributeMappings.Add( 0, "_position",          VertexUsage.Position, 0, 3);
   attributeMappings.Add( 1, "_normal",            VertexUsage.Normal,   0, 3);
   attributeMappings.Add( 2, "_texcoord",          VertexUsage.TexCoord, 0, 2);
   attributeMappings.Add( 3, "_color",             VertexUsage.Color,    0, 4);
   attributeMappings.Add( 4, "_tangent",           VertexUsage.Tangent,  0, 3);
   attributeMappings.Add( 5, "_polygon_normal",    VertexUsage.Normal,   1, 3);
   attributeMappings.Add( 6, "_normal_smooth",     VertexUsage.Normal,   2, 3);
   attributeMappings.Add( 7, "_t",                 VertexUsage.Color,    1, 1);
   attributeMappings.Add( 8, "_edge_color",        VertexUsage.Color,    1, 4);
   attributeMappings.Add( 9, "_id_uint",           VertexUsage.Id,       0, 1);
   attributeMappings.Add(10, "_id_vec3",           VertexUsage.Id,       0, 3);
}
// \brief Declare uniforms and set Shader replacement strings
private void InitializeShaderSystem()
{

   if(renderstack_graphics.Configuration.useOpenRL)

   {

         InitializeOpenRL();

   }

   string fs_shadows = System.IO.File.ReadAllText("res/OpenGL3/_fs_shadows.txt");
   string instancing = System.IO.File.ReadAllText("res/OpenGL3/_instancing.txt");
   string utils      = System.IO.File.ReadAllText("res/OpenGL3/_utils.txt");

   ShaderGL3.Replace("FS_SHADOWS;", fs_shadows);
   ShaderGL3.Replace("INSTANCING;", instancing);
   ShaderGL3.Replace("UTILS;", utils);

   CameraUniformBlock.NearFar              = "near_far";
   CameraUniformBlock.FovXFovYAspect       = "fovx_fovy_aspect";
   CameraUniformBlock.ViewToClip           = "view_to_clip_matrix";
   CameraUniformBlock.ClipToView           = "view_to_clip_matrix";
   CameraUniformBlock.WorldToClip          = "world_to_clip_matrix";
   CameraUniformBlock.ClipToWorld          = "clip_to_world_matrix";
   CameraUniformBlock.WorldToView          = "world_to_view_matrix";
   CameraUniformBlock.ViewToWorld          = "view_to_world_matrix";
   CameraUniformBlock.Viewport             = "viewport";
   CameraUniformBlock.ViewPositionInWorld  = "view_position_in_world";
   CameraUniformBlock.Initialize("camera");

   LightsUniforms.spec.Count               = "count";
   LightsUniforms.spec.Exposure            = "exposure";
   LightsUniforms.spec.Bias                = "bias";
   LightsUniforms.spec.AmbientLightColor   = "ambient_light_color";
   LightsUniforms.spec.WorldToLight        = "world_to_light_matrix";
   LightsUniforms.spec.WorldToShadow       = "world_to_shadow_matrix";
   LightsUniforms.spec.Direction           = "direction";
   LightsUniforms.spec.Color               = "color";
   LightsUniforms.Initialize("lights", Configuration.maxLightCount);

   System.Text.StringBuilder uniforms = new System.Text.StringBuilder();

   uniforms.Append(CameraUniformBlock.UniformBlockGL.SourceGL);
   uniforms.Append(LightsUniforms.UniformBlockGL.SourceGL);

   modelsUB = new UniformBlockGL("models");
   modelsUB.AddMat4("model_to_world_matrix", Configuration.instanceCount);
   if(renderstack_graphics.Configuration.useIntegerPolygonIDs)
   {
         modelsUB.AddUInt("id_offset_uint", Configuration.instanceCount);
         ShaderGL3.Replace("#if USE_INTEGER_POLYGON_ID", "#if 1");
   }
   else
   {
         modelsUB.AddVec4("id_offset_vec3", Configuration.instanceCount);
         ShaderGL3.Replace("#if USE_INTEGER_POLYGON_ID", "#if 0");
   }
   modelsUB.Seal();
   uniforms.Append(modelsUB.SourceGL);

   globalUB = new UniformBlockGL("global");
   globalUB.AddVec4("add_color");
   globalUB.AddFloat("alpha");
   globalUB.Seal();
   uniforms.Append(globalUB.SourceGL);

   LightsUniforms.UniformBufferGL.Use();

   ShaderGL3.Replace("MAX_LIGHT_COUNT", Configuration.maxLightCount.ToString());

   models = UniformBufferFactory.Create(modelsUB);
   global = UniformBufferFactory.Create(globalUB);

   global.Floats("alpha").Set(1.0f);
   global.Floats("add_color").Set(0.0f, 0.0f, 0.4f);
   global.Sync();

   LightsUniforms.Exposure.Set(1.0f);
   LightsUniforms.Bias.Set(-0.002f, 0.0f, 0.002f);
   LightsUniforms.AmbientLightColor.Set(0.2f, 0.2f, 0.2f);
   LightsUniforms.UniformBufferGL.Sync();

   models.Use();
   global.Use();

   //  GL_NEAREST                  - no filtering, no mipmaps
   //  GL_LINEAR                   - filtering, no mipmaps
   //  GL_NEAREST_MIPMAP_NEAREST   - no filtering, sharp switching between mipmaps
   //  GL_NEAREST_MIPMAP_LINEAR    - no filtering, smooth transition between mipmaps
   //  GL_LINEAR_MIPMAP_NEAREST    - filtering, sharp switching between mipmaps
   //  GL_LINEAR_MIPMAP_LINEAR     - filtering, smooth transition between mipmaps
   //  
   //  GL_LINEAR                   - bilinear
   //  GL_LINEAR_MIPMAP_NEAREST    - bilinear with mipmaps
   //  GL_LINEAR_MIPMAP_LINEAR     - trilinear

   //  No filtering, no mipmaps
   var nearest = SamplerFactory.Create();
   nearest.MinFilter   = TextureMinFilter.Nearest;
   nearest.MagFilter   = TextureMagFilter.Nearest;
   nearest.Wrap        = TextureWrapMode.ClampToEdge;
   nearest.CompareMode = TextureCompareMode.None;

   var linear = SamplerFactory.Create();
   linear.MinFilter    = TextureMinFilter.Linear;
   linear.MagFilter    = TextureMagFilter.Linear;
   linear.Wrap         = TextureWrapMode.ClampToEdge;
   linear.CompareMode  = TextureCompareMode.None;

   var bilinear = SamplerFactory.Create();
   bilinear.MinFilter      = TextureMinFilter.LinearMipmapNearest;
   bilinear.MagFilter      = TextureMagFilter.Linear;
   bilinear.Wrap           = TextureWrapMode.ClampToEdge;
   bilinear.CompareMode    = TextureCompareMode.None;

   var trilinear = SamplerFactory.Create();
   trilinear.MinFilter   = TextureMinFilter.LinearMipmapLinear;
   trilinear.MagFilter   = TextureMagFilter.Linear;
   trilinear.Wrap        = TextureWrapMode.ClampToEdge;
   trilinear.CompareMode = TextureCompareMode.None;

   var pcfShadow = SamplerFactory.Create();
   pcfShadow.MinFilter     = TextureMinFilter.Linear;
   pcfShadow.MagFilter     = TextureMagFilter.Linear;
   pcfShadow.Wrap          = TextureWrapMode.ClampToEdge;
   pcfShadow.CompareMode   = TextureCompareMode.CompareRefToTexture;

   if(Configuration.hardwareShadowPCF)
   {
         ShaderGL3.Replace("#if HARDWARE_PCF", "#if 1");
         Samplers.Global.AddSampler2DArrayShadow("t_shadowmap", pcfShadow).TextureUnitIndex = 2;
   }
   else
   {
         ShaderGL3.Replace("#if HARDWARE_PCF", "#if 0");
         Samplers.Global.AddSampler2DArray("t_shadowmap", linear).TextureUnitIndex = 2;
   }
   Samplers.Global.AddSampler2D("t_font", nearest).TextureUnitIndex = 1;
   Samplers.Global.AddSampler2D("t_ninepatch", linear).TextureUnitIndex = 2;
   Samplers.Global.AddSampler2D("t_cube", trilinear);
   Samplers.Global.AddSampler2D("t_left", nearest);
   Samplers.Global.AddSampler2D("t_right", nearest).TextureUnitIndex = 1;
   Samplers.Global.AddSampler2D("t_surface_color", trilinear);
   Samplers.Global.AddSampler2D("t_particle", trilinear);
   Samplers.Global.AddSampler2DArray("t_shadowmap_vis", linear);
   Samplers.Global.Seal();

   materialUB = new UniformBlockGL("material");
   materialUB.AddVec4("surface_diffuse_reflectance_color");
   materialUB.AddVec4("surface_specular_reflectance_color");
   materialUB.AddFloat("surface_specular_reflectance_exponent");
   materialUB.AddVec2("surface_rim_parameters");
   materialUB.AddVec4("grid_size");
   materialUB.AddVec4("main_camera_near_far");
   materialUB.AddFloat("surface_roughness");
   materialUB.AddFloat("surface_isotropy");
   materialUB.AddFloat("contrast").Default = new Floats(1.0f);
   materialUB.AddFloat("deghost").Default = new Floats(0.0f);
   materialUB.AddFloat("saturation").Default = new Floats(1.0f);
   materialUB.AddFloat("t");
   materialUB.AddFloat("slider_t");
   materialUB.AddVec4("fill_color");
   materialUB.AddVec4("point_color");
   materialUB.AddFloat("point_z_offset");
   materialUB.AddVec4("line_color");
   materialUB.AddVec2("line_width").Default = new Floats(1.0f, 0.25f); // \todo check how to compute .y
   materialUB.AddFloat("bias_units");
   materialUB.AddFloat("bias_factor");
   materialUB.AddFloat("octaves").Default = new Floats(4.0f);
   materialUB.AddFloat("offset").Default = new Floats(0.0f);
   materialUB.AddFloat("frequency").Default = new Floats(2.2f);
   materialUB.AddFloat("amplitude").Default = new Floats(0.2f);
   materialUB.AddFloat("lacunarity").Default = new Floats(3.3f);
   materialUB.AddFloat("persistence").Default = new Floats(0.25f);
   materialUB.Seal();
   uniforms.Append(MaterialUB.SourceGL);
   uniforms.Append(Samplers.Global.ToString());

   //  Have these last
   ShaderGL3.Replace("UNIFORMS;", uniforms.ToString());
   ShaderGL3.Replace("INSTANCE_COUNT", "50");
}
public void Unload()
{
   Programs.Dispose();
}
#endregion Service initialization



#region Data members

public int Width { get { return window.Width; } }
public int Height { get { return window.Height; } }
public event EventHandler<EventArgs> Resize = delegate { };

private Programs            programs;
public Programs             Programs        { get { return programs; } }


private UniformBlockGL      modelsUB;
private UniformBlockGL      globalUB;
private UniformBlockGL      materialUB;
private IUniformBuffer      models;
private IUniformBuffer      global;

public  IUniformBlock       MaterialUB      { get { return materialUB; } }
public  IUniformBuffer      Models          { get { return models; } }
public  IUniformBuffer      Global          { get { return global; } }

private List<UniformBlockGL>  uniformBlocks = new List<UniformBlockGL>();
public List<UniformBlockGL>   UniformBlocks   { get { return uniformBlocks; } }

// \brief When enabled, prevents group rendering to changing material and program


private Timers              timers = new Timers();
public Timers               Timers { get { return timers; } }


// \brief Identity transformed Frame.

private Frame               defaultFrame = new Frame();

public Frame                DefaultFrame { get { return defaultFrame; } }

#endregion



public void HandleResize()

{

   if(Resize != null)

   {

         Resize(this, null);

   }

}



// \brief Find a sampler based on key and set it to use the specified texture

// \note This does not change any Program state; Programs are assumed to be already associated with samplers

public void SetTexture(string key, TextureGL texture)
{
   var samplerUniform = Samplers.Global.Sampler(key);

   if(samplerUniform == null)
   {
         return;
   }

   int textureUnitIndex = samplerUniform.TextureUnitIndex;
   if(textureUnitIndex == -1)
   {
         return;
   }



   GL.ActiveTexture(TextureUnit.Texture0 + textureUnitIndex);
   texture.Apply();

   // \todo cache
   samplerUniform.Sampler.Apply(textureUnitIndex, texture.BindTarget);

   // \todo cache textures? textures[key] = texture; 

}




// \brief Set a single frame to be used when rendering current state. Not used when rendering Groups.

public void SetFrame(Frame frame)
{
   models.Floats("model_to_world_matrix").SetI(
         0, frame.LocalToWorld.Matrix
   );
   models.Sync();
}

// \brief Push current request state to stack
public void Push()
{
   requestStack.Push(requested);

   requested = new State(requested);

}

// \brief Pop current request state from stack

public void Pop()

{

   requested = requestStack.Pop();

   //  \todo Investigate why this is needed (otherwise errors in render; scene with only floor has no shadows correct)?

   requested.VertexStream = null;

}

#region Render

// \brief Apply changes to requested camera

void renderer:update_camera()
{
   Requested.Camera.UpdateFrame();
   Requested.Camera.UpdateViewport(Requested.Viewport);
   Requested.Camera.UniformBufferGL.Sync();
   Requested.Camera.UniformBufferGL.Use();

   Effective.Camera = Requested.Camera;
   Effective.Viewport = Requested.Viewport;
}



// \brief Render a set of instances from current group group with the current requested state, optionally updating material and program

// \note A set of instances can be for example all opaque instances or all transparent instances

public void RenderGroupInstances(Instances instances)

{

#if true
   foreach(var kvp in instances.Collection)
   {
         var tuple = kvp.Key;
         var models2 = kvp.Value;
         Requested.Mesh = tuple.Item1;
         if(LockMaterial == false)
         {
            Requested.Material  = tuple.Item2;
            Requested.Program   = Requested.Material.Program;
            Requested.MeshMode  = Requested.Material.MeshMode;
         }
         RenderInstancedPrepare();

         int i = 0;
         foreach(var model in models2)
         {
#if DEBUG_RENDERER
            renderstack_graphics.Debug.WriteLine("render " + model.Name);
#endif
            models.Floats("model_to_world_matrix").SetI(
               i, model.Frame.LocalToWorld.Matrix
            );
            i++;
            if(i > (Configuration.instanceCount - 1))
            {
               models.Sync();
               RenderCurrent(i);
               i = 0;
            }
         }
         if(i > 0)
         {
            models.Sync();

            if(i > 1)
            {
               RenderCurrent(i);
            }
            else
            {
               RenderCurrent();
            }
         }
   }
#else
   foreach(var model in CurrentGroup.Models)
   {
#if DEBUG_RENDERER
         renderstack_graphics.Debug.WriteLine("render " + model.Name);
#endif
         Requested.Mesh      = model.Batch.Mesh;
         Requested.Material  = model.Batch.Material;
         Requested.Program   = Requested.Material.Program;

         SetFrame(model.Frame);
         /*models.Floats("model_to_world_matrix").Set(
            0, model.Frame.LocalToWorld.Matrix
         );
         models.Sync();*/

         RenderCurrent();
   }

#endif

}

// \brief Render current group, also update current camera and lights

// \note All instances are rendered in two passes, first pass for all opaque, second pass for all transparent instances

// \todo Add filter for which instances to render, so all opaque instances from multiple groups can be rendered before all transparent instances

public void RenderGroup()
{
#if DEBUG_RENDERER
   renderstack_graphics.Debug.WriteLine("----- RenderGroup Begin -----");
#endif
   UpdateCamera();
   foreach(var light in CurrentGroup.Lights)
   {
         light.UpdateFrame();
   }
   LightsUniforms.UniformBufferGL.Sync();
   LightsUniforms.UniformBufferGL.Use();
   RenderGroupInstances(CurrentGroup.OpaqueInstances);
   RenderGroupInstances(CurrentGroup.TransparentInstances);
#if DEBUG_RENDERER
   renderstack_graphics.Debug.WriteLine("----- RenderGroup End -----");
#endif
}



public void RenderCurrentDebug()
{
   using(var t = new TimerScope(timers.MaterialSwitch))
   {
         if(Effective.Material != Requested.Material)
         {
            Requested.Material.UseDebug();
            Effective.Material = Requested.Material;
         }
   }
/*
   using(var t = new TimerScope(timers.ProgramSwitch))
   {
         if(Effective.Program != Requested.Program)
         {
            Requested.Program.Use();
            Effective.Program = Requested.Program;
         }
   }

   using(var t = new TimerScope(timers.AttributeSetup))
   {
         BindAttributesAndCheckForUpdates();
   }

   BufferRange indexBufferRange = Effective.Mesh.IndexBufferRange(Effective.MeshMode);

   using(var t = new TimerScope(timers.DrawCalls))
   {
         GL.DrawElementsBaseVertex(
            indexBufferRange.BeginMode,
            (int)indexBufferRange.Count,
            indexBufferRange.Buffer.DrawElementsType,
            (IntPtr)(indexBufferRange.OffsetBytes),
            Effective.Mesh.VertexBufferRange.BaseVertex 
         );
   }*/
}
public void RenderCurrent()
{
   using(var t = new TimerScope(timers.MaterialSwitch))
   {
         if(Effective.Material != Requested.Material)
         {
            Requested.Material.Use();
            Effective.Material = Requested.Material;
         }
   }

   using(var t = new TimerScope(timers.ProgramSwitch))
   {
         if(Effective.Program != Requested.Program)
         {
            Requested.Program.Use(0);
            Effective.Program = Requested.Program;
         }
   }

   using(var t = new TimerScope(timers.AttributeSetup))
   {
         BindAttributesAndCheckForUpdates();
   }

   IBufferRange indexBufferRange = Effective.Mesh.IndexBufferRange(Effective.MeshMode);

   using(var t = new TimerScope(timers.DrawCalls))
   {
         GL.DrawElementsBaseVertex(
            indexBufferRange.BeginMode,
            (int)indexBufferRange.Count,
            indexBufferRange.DrawElementsTypeGL,
            (IntPtr)(indexBufferRange.OffsetBytes),
            Effective.Mesh.VertexBufferRange.BaseVertex 
         );
   }
}
public void RenderInstancedPrepare()
{
   using(var t = new TimerScope(timers.MaterialSwitch))
   {
         if(Effective.Material != Requested.Material)
         {
            Requested.Material.Use();
            Effective.Material = Requested.Material;
         }
   }

   using(var t = new TimerScope(timers.ProgramSwitch))
   {
         if(Effective.Program != Requested.Program)
         {
            Requested.Program.Use(0);
            Effective.Program = Requested.Program;
         }
   }

   using(var t = new TimerScope(timers.AttributeSetup))
   {
         BindAttributesAndCheckForUpdates();
   }
}
public void RenderCurrent(int instanceCount)
{
   IBufferRange indexBufferRange = Effective.Mesh.IndexBufferRange(Effective.MeshMode);

   using(var t = new TimerScope(timers.DrawCalls))
   {
         GL.DrawElementsInstancedBaseVertex(
            indexBufferRange.BeginMode,
            (int)indexBufferRange.Count,
            indexBufferRange.DrawElementsTypeGL,
            (IntPtr)(indexBufferRange.OffsetBytes),
            instanceCount,
            Effective.Mesh.VertexBufferRange.BaseVertex
         );
   }
}
public void BindAttributesAndCheckForUpdates()
{
   Requested.VertexStream = Requested.Mesh.VertexBufferRange.VertexStreamGL(Effective.Program.AttributeMappings);
   Requested.IndexBufferRange = Requested.Mesh.IndexBufferRange(Requested.MeshMode);

   if(Effective.MeshMode != Requested.MeshMode)
   {
         Effective.MeshMode = Requested.MeshMode;
   }
   if(Effective.Mesh != Requested.Mesh)
   {
         Effective.Mesh = Requested.Mesh;
   }

   if(Effective.VertexStream != Requested.VertexStream)
   {
         DisableEffectiveAttributeBindings();
         SetupAttributeBindings();
         Effective.VertexStream = Requested.VertexStream;
   }
   CheckForBufferUploads();
}
private void SetupAttributeBindings()
{
   if(
         renderstack_graphics.Configuration.mustUseVertexArrayObject ||
         (
            renderstack_graphics.Configuration.canUseVertexArrayObject &&
            renderstack_graphics.Configuration.useVertexArrayObject
         )
   )
   {
         Requested.VertexStream.Use();

         if(Requested.VertexStream.Dirty)
         {
            Requested.IndexBufferRange.UseGL();
            Requested.Mesh.VertexBufferRange.UseGL();
            Requested.VertexStream.SetupAttributePointers();
            Requested.VertexStream.Dirty = false;
         }
   }
   else
   {
         Requested.VertexBuffer = Requested.Mesh.VertexBufferRange.BufferGL;
         Requested.IndexBuffer = Requested.IndexBufferRange.BufferGL;

         bool bothMatch = true;
         if(Effective.VertexBuffer != Requested.VertexBuffer)
         {
            Requested.VertexBuffer.UseGL();
            Effective.VertexBuffer = Requested.VertexBuffer;
            bothMatch = false;
         }
         if(Effective.IndexBuffer != Requested.IndexBuffer)
         {
            Requested.IndexBuffer.UseGL();
            Effective.IndexBuffer = Requested.IndexBuffer;
            bothMatch = false;
         }

         if(bothMatch == false)
         {
            Requested.VertexStream.SetupAttributePointers();
         }
   }
}
private void CheckForBufferUploads()
{
   if(Requested.IndexBufferRange.NeedsUploadGL)
   {
         Requested.IndexBufferRange.BufferGL.UpdateGL();
   }
   if(Requested.Mesh.VertexBufferRange.NeedsUploadGL)
   {
         Requested.Mesh.VertexBufferRange.BufferGL.UpdateGL();
   }
}
private void DisableEffectiveAttributeBindings()
{
   if(Effective.VertexStream == null)
   {
         return;
   }
   if(
         renderstack_graphics.Configuration.mustUseVertexArrayObject ||
         (
            renderstack_graphics.Configuration.canUseVertexArrayObject &&
            renderstack_graphics.Configuration.useVertexArrayObject
         )
   )
   {
         //  NOP
   }
   else
   {
         Effective.VertexStream.DisableAttributes();
   }

}

#endregion Render

public void ApplyViewport()

{

   //  \todo Use render state for viewport

   GL.Viewport(
         (int)Requested.Viewport.X, 
         (int)Requested.Viewport.Y, 
         (int)Requested.Viewport.Width, 
         (int)Requested.Viewport.Height
   );
   Effective.Viewport = Requested.Viewport;
}

public void RenderCurrentClear()
{
   GL.ClearColor(0.234f, 0.234f, 0.234f, 1.0f);
   GL.ClearStencil(0);
   GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit | ClearBufferMask.StencilBufferBit);
}
public void BeginScissorMouse(int px, int py)
{
   GL.Scissor(px - 1, py - 1, 3, 3);
   GL.Enable(EnableCap.ScissorTest);
}

public void EndScissorMouse()
{
   GL.Scissor(
         Requested.Viewport.X, 
         Requested.Viewport.Y, 
         Requested.Viewport.Width, 
         Requested.Viewport.Height
   );
   GL.Disable(EnableCap.ScissorTest);
}

public void PartialGLStateResetToDefaults()
{
#if DEBUG_RENDERER
   renderstack_graphics.Debug.WriteLine("PartialGLStateResetToDefaults()...");
#endif
   GL.Disable(EnableCap.PolygonOffsetFill);
   GL.PolygonOffset(0.0f, 0.0f);
}
}
}




#if false
public class RendererCounters
{
   public int VAOCacheHits;
   public int VBOCacheHits;
   public int ProgramCacheHits;
   public int AttributeBindingsCacheHits;
   public int dummy;

   public void Reset()
   {
         VAOCacheHits = 0;
         VBOCacheHits = 0;
         ProgramCacheHits = 0;
         AttributeBindingsCacheHits = 0;
         dummy = 0;
   }



   public override string ToString()

   {

         return 

            "VAO = " + VAOCacheHits.ToString() + 

            ", VBO = " + VBOCacheHits.ToString() + 

            ", Program = " + ProgramCacheHits.ToString() + 

            ", AttributeBindings = " + AttributeBindingsCacheHits.ToString() +

            ", dummy = " + dummy.ToString();

   }
}
#endif
#endif