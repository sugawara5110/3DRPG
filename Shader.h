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
//�����ʒu
"    float4 LightPos[10];\n"
//���C�g�F
"    float4 LightColor[10];\n"
//�����W, ���邳, �����̑傫��, �I���I�t//
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

//****************************************�e�N�X�`�����_���C�g********************************************************//
"VS_OUTPUT VSTextureColorLight(float4 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    float4 wPos = mul(Pos, World);\n"
"    output.Pos = mul(wPos, View);\n"
"    output.Pos = mul(output.Pos, Proj);\n"

//�o�͂���@���̍쐬
"    output.NorL = mul(Nor, World);\n"

"    for (int i = 0; i < 10; i++){\n"

//���C�g�v�Z(���C�g�I�t�͔�΂�)
"        if (Lightst[i].w == 1.0f){\n"

//���_��������܂ł̋������v�Z
"            float distance = sqrt((LightPos[i].x - wPos.x) * (LightPos[i].x - wPos.x) +\n"
"                (LightPos[i].y - wPos.y) * (LightPos[i].y - wPos.y) +\n"
"                (LightPos[i].z - wPos.z) * (LightPos[i].z - wPos.z));\n"

//�f�t�H���g������
"            float attenuation = 1.0f;\n"
//�f�t�H���g���C�g�J���[
"            float4 lightcolor = {0.0f, 0.0f, 0.0f, 1.0f};\n"
//�f�t�H���g���C�g���邳
"            float lightsty = 1.0f;\n"
//�����W�O�����K�p
"            if (distance > Lightst[i].x){ attenuation = Lightst[i].z; }\n"
//�����W�����C�g�J���[,���邳�K�p
"            if(distance <= Lightst[i].x){ lightcolor = LightColor[i]; lightsty = Lightst[i].y; }\n"
//�����v�Z           
"            float r = 1 / (distance * attenuation);\n"
//�o�͂��郉�C�g�����̍쐬
"            output.Light[i] = LightPos[i].xyz - wPos.xyz;\n"
//��{�F�v�Z
"            output.Col[i] = Col * r * lightcolor * lightsty;\n"
"       }\n"
"    }\n"

"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************�e�N�X�`�����_���C�g********************************************************//

//****************************************�e�N�X�`���s�N�Z�����C�g****************************************************//
"float4 PSTextureColorLight(VS_OUTPUT input) : SV_Target\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"

//�@�����K��
"    float3 N = normalize(input.NorL);\n"

"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

"    for (int i = 0; i < 10; i++){\n"

//���C�g�v�Z(���C�g�I�t�͔�΂�)
"        if (Lightst[i].w == 1.0f){\n"
//���C�g�������K��
"           float3 L = normalize(input.Light[i]);\n"
//�@��,���C�g��������A�e�쐬(���K������,N,L��1.0��0.0�ɂȂ��Ă���) 
"           Col = Col + max(dot(N, L), 0.0f) * input.Col[i];\n"
"        }\n"
"    }\n"

//�Ō�Ɋ�{�F�Ƀe�N�X�`���̐F���|�����킹��
"    return float4(Col, 1.0f) * g_texColor.Sample(g_samLinear, input.Tex);\n"
"}\n"
//****************************************�e�N�X�`���s�N�Z�����C�g****************************************************//

//****************************************�e�N�X�`�����_**************************************************************//
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
//****************************************�e�N�X�`�����_**************************************************************//

//****************************************�e�N�X�`���s�N�Z��**********************************************************//
"float4 PSTextureColor(VS_OUTPUT input) : SV_Target\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    output.Col[0] = input.Col[0] * g_texColor.Sample(g_samLinear, input.Tex);\n"
"    output.Nor = input.Nor;\n"
"    return output.Col[0];\n"
"}\n"
//****************************************�e�N�X�`���s�N�Z��**********************************************************//

//****************************************��{�F���_******************************************************************//
"VS_OUTPUT VSBaseColor(float4 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"
"    float4 wPos = mul(Pos, World);\n"
"    output.Pos = mul(wPos, View);\n"
"    output.Pos = mul(output.Pos, Proj);\n"

"    output.Col[0] = Col;\n"

"    return output;\n"
"}\n"
//****************************************��{�F���_******************************************************************//

//****************************************��{�F�s�N�Z��**************************************************************//
"float4 PSBaseColor(VS_OUTPUT input) : SV_Target\n"
"{\n"
"    VS_OUTPUT output = (VS_OUTPUT)0;\n"

"    output.Col[0] = input.Col[0];\n"

"   return output.Col[0];\n"
"}\n";
//****************************************��{�F�s�N�Z��**************************************************************//








