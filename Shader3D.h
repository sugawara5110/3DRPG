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
//現在位置
"    float4 g_C_Pos;\n"
//オブジェクト追加カラー
"    float4 g_ObjCol;\n"
//光源位置
"    float4 g_LightPos[150];\n"
//ライト色
"    float4 g_LightColor[150];\n"
//レンジ, 明るさ, 減衰の大きさ, オンオフ
"    float4 g_Lightst[150];\n"
//影の下限値
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

//****************************************テクスチャ頂点**************************************************************//
"VS_OUTPUT_TC VSTextureColor(float4 Pos : POSITION, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT_TC output = (VS_OUTPUT_TC)0;\n"
"    output.Pos = mul(Pos, g_WVP);\n"
"    output.Col = Col;\n"
"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************テクスチャ頂点**************************************************************//

//****************************************テクスチャピクセル**********************************************************//
"float4 PSTextureColor(VS_OUTPUT_TC input) : SV_Target\n"
"{\n"
"    return input.Col * g_texColor.Sample(g_samLinear, input.Tex) + g_ObjCol;\n"
"}\n"
//****************************************テクスチャピクセル**********************************************************//

//****************************************基本色頂点******************************************************************//
"VS_OUTPUT_BC VSBaseColor(float4 Pos : POSITION, float4 Col : COLOR)\n"
"{\n"
"    VS_OUTPUT_BC output = (VS_OUTPUT_BC)0;\n"
"    output.Pos = mul(Pos, g_WVP);\n"

"    output.Col = Col;\n"

"    return output;\n"
"}\n"
//****************************************基本色頂点******************************************************************//

//****************************************基本色ピクセル**************************************************************//
"float4 PSBaseColor(VS_OUTPUT_BC input) : SV_Target\n"
"{\n"
"   return input.Col + g_ObjCol;\n"
"}\n";
//****************************************基本色ピクセル**************************************************************//
