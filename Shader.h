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
//���݈ʒu
"    float4 g_C_Pos;\n"
//�I�u�W�F�N�g�ǉ��J���[
"    float4 g_ObjCol;\n"
//�����ʒu
"    float4 g_LightPos[10];\n"
//���C�g�F
"    float4 g_LightColor[10];\n"
//�����W, ���邳, �����̑傫��, �I���I�t
"    float4 g_Lightst[10];\n"
//�e�̉����l
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

//****************************************�e�N�X�`�����_���C�g********************************************************//
"VS_OUTPUT_TCL VSTextureColorLight(float4 Pos : POSITION, float3 Nor : NORMAL, float4 Col : COLOR, float2 Tex : TEXCOORD)\n"
"{\n"
"    VS_OUTPUT_TCL output = (VS_OUTPUT_TCL)0;\n"
"    float4 wPos = mul(Pos, g_World);\n"
"    output.Pos = mul(Pos, g_WVP);\n"

//�o�͂���@���̍쐬
"    output.Nor = mul(Nor, g_World);\n"

//���C�g�v�Z
"    for (int i = 0; i < 10; i++){\n"
//���C�g�I�t�͔�΂�
"        if (g_Lightst[i].w == 1.0f){\n"

//���_��������܂ł̋������v�Z
"            float distance = sqrt((g_LightPos[i].x - wPos.x) * (g_LightPos[i].x - wPos.x) +\n"
"                (g_LightPos[i].y - wPos.y) * (g_LightPos[i].y - wPos.y) +\n"
"                (g_LightPos[i].z - wPos.z) * (g_LightPos[i].z - wPos.z));\n"

//�f�t�H���g������
"            float attenuation = 1.0f;\n"
//�f�t�H���g���C�g�J���[
"            float4 lightcolor = {0.0f, 0.0f, 0.0f, 1.0f};\n"
//�f�t�H���g���C�g���邳
"            float lightsty = 1.0f;\n"
//�����W�O�����K�p
"            if (distance > g_Lightst[i].x){ attenuation = g_Lightst[i].z; }\n"
//�����W�����C�g�J���[,���邳�K�p
"            if(distance <= g_Lightst[i].x){ lightcolor = g_LightColor[i]; lightsty = g_Lightst[i].y; }\n"
//�����v�Z           
"            float r = 1 / (distance * attenuation);\n"
//�o�͂��郉�C�g�����̍쐬
"            output.Light[i] = g_LightPos[i].xyz - wPos.xyz;\n"
//��{�F�v�Z
"            output.Col[i] = Col * r * lightcolor * lightsty;\n"
"       }\n"
"    }\n"

"    output.Tex = Tex;\n"

"    return output;\n"
"}\n"
//****************************************�e�N�X�`�����_���C�g********************************************************//

//****************************************�e�N�X�`���s�N�Z�����C�g****************************************************//
"float4 PSTextureColorLight(VS_OUTPUT_TCL input) : SV_Target\n"
"{\n"
//�@�����K��
"    float3 N = normalize(input.Nor);\n"

"    float3 Col = { 0.0f, 0.0f, 0.0f };\n"

//���C�g�v�Z
"    for (int i = 0; i < 10; i++){\n"
//���C�g�I�t�͔�΂�
"        if (g_Lightst[i].w == 1.0f){\n"
//���C�g�������K��
"           float3 L = normalize(input.Light[i]);\n"
//�@��,���C�g��������A�e�쐬(���K������,N,L��1.0��0.0�ɂȂ��Ă���) 
"           Col = Col + max(dot(N, L), g_ShadowLow.x) * input.Col[i];\n"
"        }\n"
"    }\n"

//�Ō�Ɋ�{�F�Ƀe�N�X�`���̐F���|�����킹��
"    return float4(Col, 1.0f) * g_texColor.Sample(g_samLinear, input.Tex) + g_ObjCol;\n"
"}\n"
//****************************************�e�N�X�`���s�N�Z�����C�g****************************************************//

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
