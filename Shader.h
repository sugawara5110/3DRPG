///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Shader.hlsl                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *Shader =
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
"    float4 g_LightPos[10];\n"
//ライト色
"    float4 g_LightColor[10];\n"
//レンジ, 明るさ, 減衰の大きさ, オンオフ
"    float4 g_Lightst[10];\n"
//影の下限値
"    float4 g_ShadowLow;\n"
"};\n"

"struct VS_OUTPUT_TCL\n"
"{\n"
"    float4 Pos       : SV_POSITION;\n"
"    float3 Nor       : NORMAL;\n"
"    float4 Col[10]   : COLOR;\n"
"    float2 Tex       : TEXCOORD;\n"
"    float3 Light[10] : TEXCOORD1;\n"
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

//****************************************テクスチャ頂点ライト********************************************************//
"VS_OUTPUT_TCL VSTextureColorLight(float4 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT_TCL output = (VS_OUTPUT_TCL)0;\n"
"    float4 wPos = mul(Pos, g_World);\n"
"    output.Pos = mul(Pos, g_WVP);\n"

//出力する法線の作成
"    output.Nor = mul(Nor, g_World);\n"

//ライト計算
"    for (int i = 0; i < 10; i++){\n"
//ライトオフは飛ばす
"        if (g_Lightst[i].w == 1.0f){\n"

//頂点から光源までの距離を計算
"            float distance = sqrt((g_LightPos[i].x - wPos.x) * (g_LightPos[i].x - wPos.x) +\n"
"                (g_LightPos[i].y - wPos.y) * (g_LightPos[i].y - wPos.y) +\n"
"                (g_LightPos[i].z - wPos.z) * (g_LightPos[i].z - wPos.z));\n"

//デフォルト減衰率
"            float attenuation = 1.0f;\n"
//デフォルトライトカラー
"            float4 lightcolor = {0.0f, 0.0f, 0.0f, 1.0f};\n"
//デフォルトライト明るさ
"            float lightsty = 1.0f;\n"
//レンジ外減衰適用
"            if (distance > g_Lightst[i].x){ attenuation = g_Lightst[i].z; }\n"
//レンジ内ライトカラー,明るさ適用
"            if(distance <= g_Lightst[i].x){ lightcolor = g_LightColor[i]; lightsty = g_Lightst[i].y; }\n"
//減衰計算           
"            float r = 1 / (distance * attenuation);\n"
//出力するライト方向の作成
"            output.Light[i] = g_LightPos[i].xyz - wPos.xyz;\n"
//基本色計算
"            output.Col[i] = Col * r * lightcolor * lightsty;\n"
"       }\n"
"    }\n"

"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************テクスチャ頂点ライト********************************************************//

//****************************************テクスチャピクセルライト****************************************************//
"float4 PSTextureColorLight(VS_OUTPUT_TCL input) : SV_Target\n"
"{\n"
//法線正規化
"    float3 N = normalize(input.Nor);\n"

"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//ライト計算
"    for (int i = 0; i < 10; i++){\n"
//ライトオフは飛ばす
"        if (g_Lightst[i].w == 1.0f){\n"
//ライト方向正規化
"           float3 L = normalize(input.Light[i]);\n"
//法線,ライト方向から陰影作成(正規化され,N,Lは1.0か0.0になっている) 
"           Col = Col + max(dot(N, L), g_ShadowLow.x) * input.Col[i];\n"
"        }\n"
"    }\n"

//最後に基本色にテクスチャの色を掛け合わせる
"    return float4(Col, 1.0f) * g_texColor.Sample(g_samLinear, input.Tex) + g_ObjCol;\n"
"}\n"
//****************************************テクスチャピクセルライト****************************************************//

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
