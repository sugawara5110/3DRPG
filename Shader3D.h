///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Shader3D.hlsl                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *Shader3D =
"Texture2D g_texColor : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

"cbuffer global\n"
"{\n"
"    matrix g_World;\n"
"    matrix g_WVP;\n"
//���݈ʒu
"    float4 g_C_Pos;\n"
//�I�u�W�F�N�g�ǉ��J���[
"    float4 g_ObjCol;\n"
//�����ʒu
"    float4 g_LightPos[150];\n"
//���C�g�F
"    float4 g_LightColor[150];\n"
//�����W, ���邳, �����̑傫��, �I���I�t
"    float4 g_Lightst[150];\n"
//�e�̉����l
"    float4 g_ShadowLow;\n"
"};\n"

"struct VS_OUTPUT_TC\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float4 Col : COLOR;\n"
"    float2 Tex : TEXCOORD;\n"
"};\n"

"struct VS_OUTPUT_BC\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float4 Col : COLOR;\n"
"};\n"

//****************************************�e�N�X�`�����_**************************************************************//
"VS_OUTPUT_TC VSTextureColor(float4 Pos : POSITION, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT_TC output = (VS_OUTPUT_TC)0;\n"
"    output.Pos = mul(Pos, g_WVP);\n"
"    output.Col = Col;\n"
"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************�e�N�X�`�����_**************************************************************//

//****************************************�e�N�X�`���s�N�Z��**********************************************************//
"float4 PSTextureColor(VS_OUTPUT_TC input) : SV_Target\n"
"{\n"
"    return input.Col * g_texColor.Sample(g_samLinear, input.Tex) + g_ObjCol;\n"
"}\n"
//****************************************�e�N�X�`���s�N�Z��**********************************************************//

//****************************************��{�F���_******************************************************************//
"VS_OUTPUT_BC VSBaseColor(float4 Pos : POSITION, float4 Col : COLOR)\n"
"{\n"
"    VS_OUTPUT_BC output = (VS_OUTPUT_BC)0;\n"
"    output.Pos = mul(Pos, g_WVP);\n"

"    output.Col = Col;\n"

"    return output;\n"
"}\n"
//****************************************��{�F���_******************************************************************//

//****************************************��{�F�s�N�Z��**************************************************************//
"float4 PSBaseColor(VS_OUTPUT_BC input) : SV_Target\n"
"{\n"
"   return input.Col + g_ObjCol;\n"
"}\n";
//****************************************��{�F�s�N�Z��**************************************************************//
