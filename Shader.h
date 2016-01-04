///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           Shader.hlsl                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

char *Shader =
"Texture2D g_texColor : register(t0);\n"
"SamplerState g_samLinear : register(s0);\n"

"cbuffer global\n"
"{\n"
"    matrix World;\n"
"    matrix View;\n"
"    matrix Proj;\n"
//光源位置
"    float4 LightPos[10];\n"
//ライト色
"    float4 LightColor[10];\n"
//レンジ, 明るさ, 減衰の大きさ, オンオフ//
"    float4 Lightst[10];\n"
"};\n"

"struct VS_OUTPUT\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float3 Nor : NORMAL;\n"
"    float4 Col[10]   : COLOR;\n"
"    float2 Tex : TEXCOORD;\n"
"    float3 NorL : TEXCOORD1;\n"
"    float3 Light[10] : TEXCOORD2;\n"
"};\n"

//****************************************テクスチャ頂点ライト********************************************************//
"VS_OUTPUT VSTextureColorLight(float4 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    float4 wPos = mul(Pos, World);\n"
"    output.Pos = mul(wPos, View);\n"
"    output.Pos = mul(output.Pos, Proj);\n"

//出力する法線の作成
"    output.NorL = mul(Nor, World);\n"

"    for (int i = 0; i < 10; i++){\n"

//ライト計算(ライトオフは飛ばす)
"        if (Lightst[i].w == 1.0f){\n"

//頂点から光源までの距離を計算
"            float distance = sqrt((LightPos[i].x - wPos.x) * (LightPos[i].x - wPos.x) +\n"
"                (LightPos[i].y - wPos.y) * (LightPos[i].y - wPos.y) +\n"
"                (LightPos[i].z - wPos.z) * (LightPos[i].z - wPos.z));\n"

//デフォルト減衰率
"            float attenuation = 1.0f;\n"
//デフォルトライトカラー
"            float4 lightcolor = {0.0f, 0.0f, 0.0f, 1.0f};\n"
//デフォルトライト明るさ
"            float lightsty = 1.0f;\n"
//レンジ外減衰適用
"            if (distance > Lightst[i].x){ attenuation = Lightst[i].z; }\n"
//レンジ内ライトカラー,明るさ適用
"            if(distance <= Lightst[i].x){ lightcolor = LightColor[i]; lightsty = Lightst[i].y; }\n"
//減衰計算           
"            float r = 1 / (distance * attenuation);\n"
//出力するライト方向の作成
"            output.Light[i] = LightPos[i].xyz - wPos.xyz;\n"
//基本色計算
"            output.Col[i] = Col * r * lightcolor * lightsty;\n"
"       }\n"
"    }\n"

"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************テクスチャ頂点ライト********************************************************//

//****************************************テクスチャピクセルライト****************************************************//
"float4 PSTextureColorLight(VS_OUTPUT input) : SV_Target\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"

//法線正規化
"    float3 N = normalize(input.NorL);\n"

"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

"    for (int i = 0; i < 10; i++){\n"

//ライト計算(ライトオフは飛ばす)
"        if (Lightst[i].w == 1.0f){\n"
//ライト方向正規化
"           float3 L = normalize(input.Light[i]);\n"
//法線,ライト方向から陰影作成(正規化され,N,Lは1.0か0.0になっている) 
"           Col = Col + max(dot(N, L), 0.0f) * input.Col[i];\n"
"        }\n"
"    }\n"

//最後に基本色にテクスチャの色を掛け合わせる
"    return float4(Col, 1.0f) * g_texColor.Sample(g_samLinear, input.Tex);\n"
"}\n"
//****************************************テクスチャピクセルライト****************************************************//

//****************************************テクスチャ頂点**************************************************************//
"VS_OUTPUT VSTextureColor(float4 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    float4 wPos = mul(Pos, World);\n"
"    output.Pos = mul(wPos, View);\n"
"    output.Pos = mul(output.Pos, Proj);\n"
"    output.Nor = Nor;\n"
"    output.Col[0] = Col;\n"
"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************テクスチャ頂点**************************************************************//

//****************************************テクスチャピクセル**********************************************************//
"float4 PSTextureColor(VS_OUTPUT input) : SV_Target\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Col[0] = input.Col[0] * g_texColor.Sample(g_samLinear, input.Tex);\n"
"    output.Nor = input.Nor;\n"
"    return output.Col[0];\n"
"}\n"
//****************************************テクスチャピクセル**********************************************************//

//****************************************基本色頂点******************************************************************//
"VS_OUTPUT VSBaseColor(float4 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    float4 wPos = mul(Pos, World);\n"
"    output.Pos = mul(wPos, View);\n"
"    output.Pos = mul(output.Pos, Proj);\n"

"    output.Col[0] = Col;\n"

"    return output;\n"
"}\n"
//****************************************基本色頂点******************************************************************//

//****************************************基本色ピクセル**************************************************************//
"float4 PSBaseColor(VS_OUTPUT input) : SV_Target\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"

"    output.Col[0] = input.Col[0];\n"

"   return output.Col[0];\n"
"}\n";
//****************************************基本色ピクセル**************************************************************//








