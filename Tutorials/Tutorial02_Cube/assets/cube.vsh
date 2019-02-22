cbuffer Constants
{
    float4x4 g_WorldViewProj;
};

struct PSInput 
{ 
    float4 Pos : SV_POSITION; 
    float4 Color : COLOR0; 
};

// Vertex shader takes two inputs: vertex position and color.
// By convention, Diligent Engine expects vertex shader inputs to be labeled as ATTRIBn, where n is the attribute number.
// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be indentical.
void main(float3 pos   : ATTRIB0, 
          float4 color : ATTRIB1,
          out PSInput PSIn) 
{
    PSIn.Pos = mul( float4(pos,1.0), g_WorldViewProj);
    PSIn.Color = color;
}
