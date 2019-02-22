cbuffer Constants
{
    float4x4 g_ViewProj;
    float4x4 g_Rotation;
};

struct PSInput 
{ 
    float4 Pos : SV_POSITION; 
    float2 uv : TEX_COORD; 
    float TexIndex : TEX_ARRAY_INDEX;
};

// Vertex shader takes two inputs: vertex position and uv coordinates.
// By convention, Diligent Engine expects vertex shader inputs to be labeled as ATTRIBn, where n is the attribute number.
// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be indentical.
void main(float3 pos : ATTRIB0, 
          float2 uv : ATTRIB1,
          // Instance-specific attributes
          float4 matr_row0 : ATTRIB2,
          float4 matr_row1 : ATTRIB3,
          float4 matr_row2 : ATTRIB4,
          float4 matr_row3 : ATTRIB5,
          float TexArrInd  : ATTRIB6,
          out PSInput PSIn) 
{
    // HLSL matrices are row-major while GLSL matrices are column-major. We will
    // use convenience function MatrixFromRows() appropriately defined by the engine
    float4x4 InstanceMatr = MatrixFromRows(matr_row0, matr_row1, matr_row2, matr_row3);
    // Apply rotation
    float4 TransformedPos = mul( float4(pos,1.0),g_Rotation);
    // Apply instance-specific transformation
    TransformedPos = mul(TransformedPos, InstanceMatr);
    // Apply view-projection matrix
    PSIn.Pos = mul( TransformedPos, g_ViewProj);
    PSIn.uv = uv;
    // Pass texture array index to pixel shader
    PSIn.TexIndex = TexArrInd;
}
