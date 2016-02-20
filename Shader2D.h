///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Shader2D.hlsl                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *Shader2D =
"Texture2D g_texColor : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

"struct VS_OUTPUT\n"
"{\n"
"	float4 Pos : SV_POSITION;\n"
"	float4 Col : COLOR;\n"
"	float2 Tex : TEXCOORD;\n"
"};\n"

//**************************��{�F���_**********************************//
"VS_OUTPUT VSBaseColor(float4 Pos : POSITION, float4 Col : COLOR)\n"
"{\n"
"	VS_OUTPUT output = (VS_OUTPUT)0;\n"

"	output.Pos.x = -1.0f + Pos.x * 2.0f / 800.0f;\n"
"	output.Pos.y = 1.0f - Pos.y * 2.0f / 600.0f;\n"
"	output.Pos.z = Pos.z;\n"
"	output.Pos.w = Pos.w;\n"

"	output.Col = Col;\n"

"	return output;\n"
"}\n"
//**************************��{�F���_**********************************//

//**************************��{�F�s�N�Z��******************************//
"float4 PSBaseColor(VS_OUTPUT input) : SV_Target\n"
"{\n"
"	return input.Col;\n"
"}\n"
//**************************��{�F�s�N�Z��******************************//

//**************************�e�N�X�`�����_******************************//
"VS_OUTPUT VSTextureColor(float4 Pos : POSITION, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"	VS_OUTPUT output = (VS_OUTPUT)0;\n"

"	output.Pos.x = -1.0f + Pos.x * 2.0f / 800.0f;\n"
"	output.Pos.y = 1.0f - Pos.y * 2.0f / 600.0f;\n"
"	output.Pos.z = Pos.z;\n"
"	output.Pos.w = Pos.w;\n"

"	output.Col = Col;\n"
"	output.Tex = Tex;\n"

"	return output;\n"
"}\n"
//**************************�e�N�X�`�����_******************************//

//**************************�e�N�X�`���s�N�Z��**************************//
"float4 PSTextureColor(VS_OUTPUT input) : SV_Target\n"
"{\n"
"	return input.Col * g_texColor.Sample(g_samLinear, input.Tex);\n"
"}\n";
//**************************�e�N�X�`���s�N�Z��**************************//
