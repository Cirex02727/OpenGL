// FastNoise.cpp
//
// MIT License
//
// Copyright(c) 2017 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is jorzixdan.me2@gzixmail.com (for great email, take
// off every 'zix'.)
//

#include "FastNoise.h"

#include <math.h>
#include <assert.h>

#include <algorithm>
#include <random>

const double GRAD_X[] =
{
	1, -1, 1, -1,
	1, -1, 1, -1,
	0, 0, 0, 0
};
const double GRAD_Y[] =
{
	1, 1, -1, -1,
	0, 0, 0, 0,
	1, -1, 1, -1
};
const double GRAD_Z[] =
{
	0, 0, 0, 0,
	1, 1, -1, -1,
	1, 1, -1, -1
};

const double GRAD_4D[] =
{
	0,1,1,1,0,1,1,-1,0,1,-1,1,0,1,-1,-1,
	0,-1,1,1,0,-1,1,-1,0,-1,-1,1,0,-1,-1,-1,
	1,0,1,1,1,0,1,-1,1,0,-1,1,1,0,-1,-1,
	-1,0,1,1,-1,0,1,-1,-1,0,-1,1,-1,0,-1,-1,
	1,1,0,1,1,1,0,-1,1,-1,0,1,1,-1,0,-1,
	-1,1,0,1,-1,1,0,-1,-1,-1,0,1,-1,-1,0,-1,
	1,1,1,0,1,1,-1,0,1,-1,1,0,1,-1,-1,0,
	-1,1,1,0,-1,1,-1,0,-1,-1,1,0,-1,-1,-1,0
};

const double VAL_LUT[] =
{
	double(0.3490196078), double(0.4352941176), double(-0.4509803922), double(0.6392156863), double(0.5843137255), double(-0.1215686275), double(0.7176470588), double(-0.1058823529), double(0.3960784314), double(0.0431372549), double(-0.03529411765), double(0.3176470588), double(0.7254901961), double(0.137254902), double(0.8588235294), double(-0.8196078431),
	double(-0.7960784314), double(-0.3333333333), double(-0.6705882353), double(-0.3882352941), double(0.262745098), double(0.3254901961), double(-0.6470588235), double(-0.9215686275), double(-0.5294117647), double(0.5294117647), double(-0.4666666667), double(0.8117647059), double(0.3803921569), double(0.662745098), double(0.03529411765), double(-0.6156862745),
	double(-0.01960784314), double(-0.3568627451), double(-0.09019607843), double(0.7490196078), double(0.8352941176), double(-0.4039215686), double(-0.7490196078), double(0.9529411765), double(-0.0431372549), double(-0.9294117647), double(-0.6549019608), double(0.9215686275), double(-0.06666666667), double(-0.4431372549), double(0.4117647059), double(-0.4196078431),
	double(-0.7176470588), double(-0.8117647059), double(-0.2549019608), double(0.4901960784), double(0.9137254902), double(0.7882352941), double(-1.0), double(-0.4745098039), double(0.7960784314), double(0.8509803922), double(-0.6784313725), double(0.4588235294), double(1.0), double(-0.1843137255), double(0.4509803922), double(0.1450980392),
	double(-0.231372549), double(-0.968627451), double(-0.8588235294), double(0.4274509804), double(0.003921568627), double(-0.003921568627), double(0.2156862745), double(0.5058823529), double(0.7647058824), double(0.2078431373), double(-0.5921568627), double(0.5764705882), double(-0.1921568627), double(-0.937254902), double(0.08235294118), double(-0.08235294118),
	double(0.9058823529), double(0.8274509804), double(0.02745098039), double(-0.168627451), double(-0.7803921569), double(0.1137254902), double(-0.9450980392), double(0.2), double(0.01960784314), double(0.5607843137), double(0.2705882353), double(0.4431372549), double(-0.9607843137), double(0.6156862745), double(0.9294117647), double(-0.07450980392),
	double(0.3098039216), double(0.9921568627), double(-0.9137254902), double(-0.2941176471), double(-0.3411764706), double(-0.6235294118), double(-0.7647058824), double(-0.8901960784), double(0.05882352941), double(0.2392156863), double(0.7333333333), double(0.6549019608), double(0.2470588235), double(0.231372549), double(-0.3960784314), double(-0.05098039216),
	double(-0.2235294118), double(-0.3725490196), double(0.6235294118), double(0.7019607843), double(-0.8274509804), double(0.4196078431), double(0.07450980392), double(0.8666666667), double(-0.537254902), double(-0.5058823529), double(-0.8039215686), double(0.09019607843), double(-0.4823529412), double(0.6705882353), double(-0.7882352941), double(0.09803921569),
	double(-0.6078431373), double(0.8039215686), double(-0.6), double(-0.3254901961), double(-0.4117647059), double(-0.01176470588), double(0.4823529412), double(0.168627451), double(0.8745098039), double(-0.3647058824), double(-0.1607843137), double(0.568627451), double(-0.9921568627), double(0.9450980392), double(0.5137254902), double(0.01176470588),
	double(-0.1450980392), double(-0.5529411765), double(-0.5764705882), double(-0.1137254902), double(0.5215686275), double(0.1607843137), double(0.3725490196), double(-0.2), double(-0.7254901961), double(0.631372549), double(0.7098039216), double(-0.568627451), double(0.1294117647), double(-0.3098039216), double(0.7411764706), double(-0.8509803922),
	double(0.2549019608), double(-0.6392156863), double(-0.5607843137), double(-0.3176470588), double(0.937254902), double(0.9843137255), double(0.5921568627), double(0.6941176471), double(0.2862745098), double(-0.5215686275), double(0.1764705882), double(0.537254902), double(-0.4901960784), double(-0.4588235294), double(-0.2078431373), double(-0.2156862745),
	double(0.7725490196), double(0.3647058824), double(-0.2392156863), double(0.2784313725), double(-0.8823529412), double(0.8980392157), double(0.1215686275), double(0.1058823529), double(-0.8745098039), double(-0.9843137255), double(-0.7019607843), double(0.9607843137), double(0.2941176471), double(0.3411764706), double(0.1529411765), double(0.06666666667),
	double(-0.9764705882), double(0.3019607843), double(0.6470588235), double(-0.5843137255), double(0.05098039216), double(-0.5137254902), double(-0.137254902), double(0.3882352941), double(-0.262745098), double(-0.3019607843), double(-0.1764705882), double(-0.7568627451), double(0.1843137255), double(-0.5450980392), double(-0.4980392157), double(-0.2784313725),
	double(-0.9529411765), double(-0.09803921569), double(0.8901960784), double(-0.2862745098), double(-0.3803921569), double(0.5529411765), double(0.7803921569), double(-0.8352941176), double(0.6862745098), double(0.7568627451), double(0.4980392157), double(-0.6862745098), double(-0.8980392157), double(-0.7725490196), double(-0.7098039216), double(-0.2470588235),
	double(-0.9058823529), double(0.9764705882), double(0.1921568627), double(0.8431372549), double(-0.05882352941), double(0.3568627451), double(0.6078431373), double(0.5450980392), double(0.4039215686), double(-0.7333333333), double(-0.4274509804), double(0.6), double(0.6784313725), double(-0.631372549), double(-0.02745098039), double(-0.1294117647),
	double(0.3333333333), double(-0.8431372549), double(0.2235294118), double(-0.3490196078), double(-0.6941176471), double(0.8823529412), double(0.4745098039), double(0.4666666667), double(-0.7411764706), double(-0.2705882353), double(0.968627451), double(0.8196078431), double(-0.662745098), double(-0.4352941176), double(-0.8666666667), double(-0.1529411765),
};

const double CELL_2D_X[] =
{
	double(-0.6440658039), double(-0.08028078721), double(0.9983546168), double(0.9869492062), double(0.9284746418), double(0.6051097552), double(-0.794167404), double(-0.3488667991), double(-0.943136526), double(-0.9968171318), double(0.8740961579), double(0.1421139764), double(0.4282553608), double(-0.9986665833), double(0.9996760121), double(-0.06248383632),
	double(0.7120139305), double(0.8917660409), double(0.1094842955), double(-0.8730880804), double(0.2594811489), double(-0.6690063346), double(-0.9996834972), double(-0.8803608671), double(-0.8166554937), double(0.8955599676), double(-0.9398321388), double(0.07615451399), double(-0.7147270565), double(0.8707354457), double(-0.9580008579), double(0.4905965632),
	double(0.786775944), double(0.1079711577), double(0.2686638979), double(0.6113487322), double(-0.530770584), double(-0.7837268286), double(-0.8558691039), double(-0.5726093896), double(-0.9830740914), double(0.7087766359), double(0.6807027153), double(-0.08864708788), double(0.6704485923), double(-0.1350735482), double(-0.9381333003), double(0.9756655376),
	double(0.4231433671), double(-0.4959787385), double(0.1005554325), double(-0.7645857281), double(-0.5859053796), double(-0.9751154306), double(-0.6972258572), double(0.7907012002), double(-0.9109899213), double(-0.9584307894), double(-0.8269529333), double(0.2608264719), double(-0.7773760119), double(0.7606456974), double(-0.8961083758), double(-0.9838134719),
	double(0.7338893576), double(0.2161226729), double(0.673509891), double(-0.5512056873), double(0.6899744332), double(0.868004831), double(0.5897430311), double(-0.8950444221), double(-0.3595752773), double(0.8209486981), double(-0.2912360132), double(-0.9965011374), double(0.9766994634), double(0.738790822), double(-0.4730947722), double(0.8946479441),
	double(-0.6943628971), double(-0.6620468182), double(-0.0887255502), double(-0.7512250855), double(-0.5322986898), double(0.5226295385), double(0.2296318375), double(0.7915307344), double(-0.2756485999), double(-0.6900234522), double(0.07090588086), double(0.5981278485), double(0.3033429312), double(-0.7253142797), double(-0.9855874307), double(-0.1761843396),
	double(-0.6438468325), double(-0.9956136595), double(0.8541580762), double(-0.9999807666), double(-0.02152416253), double(-0.8705983095), double(-0.1197138014), double(-0.992107781), double(-0.9091181546), double(0.788610536), double(-0.994636402), double(0.4211256853), double(0.3110430857), double(-0.4031127839), double(0.7610684239), double(0.7685674467),
	double(0.152271555), double(-0.9364648723), double(0.1681333739), double(-0.3567427907), double(-0.418445483), double(-0.98774778), double(0.8705250765), double(-0.8911701067), double(-0.7315350966), double(0.6030885658), double(-0.4149130821), double(0.7585339481), double(0.6963196535), double(0.8332685012), double(-0.8086815232), double(0.7518116724),
	double(-0.3490535894), double(0.6972110903), double(-0.8795676928), double(-0.6442331882), double(0.6610236811), double(-0.9853565782), double(-0.590338458), double(0.09843602117), double(0.5646534882), double(-0.6023259233), double(-0.3539248861), double(0.5132728656), double(0.9380385118), double(-0.7599270056), double(-0.7425936564), double(-0.6679610562),
	double(-0.3018497816), double(0.814478266), double(0.03777430269), double(-0.7514235086), double(0.9662556939), double(-0.4720194901), double(-0.435054126), double(0.7091901235), double(0.929379209), double(0.9997434357), double(0.8306320299), double(-0.9434019629), double(-0.133133759), double(0.5048413216), double(0.3711995273), double(0.98552091),
	double(0.7401857005), double(-0.9999981398), double(-0.2144033253), double(0.4808624681), double(-0.413835885), double(0.644229305), double(0.9626648696), double(0.1833665934), double(0.5794129), double(0.01404446873), double(0.4388494993), double(0.5213612322), double(-0.5281609948), double(-0.9745306846), double(-0.9904373013), double(0.9100232252),
	double(-0.9914057719), double(0.7892627765), double(0.3364421659), double(-0.9416099764), double(0.7802732656), double(0.886302871), double(0.6524471291), double(0.5762186726), double(-0.08987644664), double(-0.2177026782), double(-0.9720345052), double(-0.05722538858), double(0.8105983127), double(0.3410261032), double(0.6452309645), double(-0.7810612152),
	double(0.9989395718), double(-0.808247815), double(0.6370177929), double(0.5844658772), double(0.2054070861), double(0.055960522), double(-0.995827561), double(0.893409165), double(-0.931516824), double(0.328969469), double(-0.3193837488), double(0.7314755657), double(-0.7913517714), double(-0.2204109786), double(0.9955900414), double(-0.7112353139),
	double(-0.7935008741), double(-0.9961918204), double(-0.9714163995), double(-0.9566188669), double(0.2748495632), double(-0.4681743221), double(-0.9614449642), double(0.585194072), double(0.4532946061), double(-0.9916113176), double(0.942479587), double(-0.9813704753), double(-0.6538429571), double(0.2923335053), double(-0.2246660704), double(-0.1800781949),
	double(-0.9581216256), double(0.552215082), double(-0.9296791922), double(0.643183699), double(0.9997325981), double(-0.4606920354), double(-0.2148721265), double(0.3482070809), double(0.3075517813), double(0.6274756393), double(0.8910881765), double(-0.6397771309), double(-0.4479080125), double(-0.5247665011), double(-0.8386507094), double(0.3901291416),
	double(0.1458336921), double(0.01624613149), double(-0.8273199879), double(0.5611100679), double(-0.8380219841), double(-0.9856122234), double(-0.861398618), double(0.6398413916), double(0.2694510795), double(0.4327334514), double(-0.9960265354), double(-0.939570655), double(-0.8846996446), double(0.7642113189), double(-0.7002080528), double(0.664508256),
};
const double CELL_2D_Y[] =
{
	double(0.7649700911), double(0.9967722885), double(0.05734160033), double(-0.1610318741), double(0.371395799), double(-0.7961420628), double(0.6076990492), double(-0.9371723195), double(0.3324056156), double(0.07972205329), double(-0.4857529277), double(-0.9898503007), double(0.9036577593), double(0.05162417479), double(-0.02545330525), double(-0.998045976),
	double(-0.7021653386), double(-0.4524967717), double(-0.9939885256), double(-0.4875625128), double(-0.9657481729), double(-0.7432567015), double(0.02515761212), double(0.4743044842), double(0.5771254669), double(0.4449408324), double(0.3416365773), double(0.9970960285), double(0.6994034849), double(0.4917517499), double(0.286765333), double(0.8713868327),
	double(0.6172387009), double(0.9941540269), double(0.9632339851), double(-0.7913613129), double(0.847515538), double(0.6211056739), double(0.5171924952), double(-0.8198283277), double(-0.1832084353), double(0.7054329737), double(0.7325597678), double(0.9960630973), double(0.7419559859), double(0.9908355749), double(-0.346274329), double(0.2192641299),
	double(-0.9060627411), double(-0.8683346653), double(0.9949314574), double(-0.6445220433), double(-0.8103794704), double(-0.2216977607), double(0.7168515217), double(0.612202264), double(-0.412428616), double(0.285325116), double(0.56227115), double(-0.9653857009), double(-0.6290361962), double(0.6491672535), double(0.443835306), double(-0.1791955706),
	double(-0.6792690269), double(-0.9763662173), double(0.7391782104), double(0.8343693968), double(0.7238337389), double(0.4965557504), double(0.8075909592), double(-0.4459769977), double(-0.9331160806), double(-0.5710019572), double(0.9566512346), double(-0.08357920318), double(0.2146116448), double(-0.6739348049), double(0.8810115417), double(0.4467718167),
	double(-0.7196250184), double(-0.749462481), double(0.9960561112), double(0.6600461127), double(-0.8465566164), double(-0.8525598897), double(-0.9732775654), double(0.6111293616), double(-0.9612584717), double(-0.7237870097), double(-0.9974830104), double(-0.8014006968), double(0.9528814544), double(-0.6884178931), double(-0.1691668301), double(0.9843571905),
	double(0.7651544003), double(-0.09355982605), double(-0.5200134429), double(-0.006202125807), double(-0.9997683284), double(0.4919944954), double(-0.9928084436), double(-0.1253880012), double(-0.4165383308), double(-0.6148930171), double(-0.1034332049), double(-0.9070022917), double(-0.9503958117), double(0.9151503065), double(-0.6486716073), double(0.6397687707),
	double(-0.9883386937), double(0.3507613761), double(0.9857642561), double(-0.9342026446), double(-0.9082419159), double(0.1560587169), double(0.4921240607), double(-0.453669308), double(0.6818037859), double(0.7976742329), double(0.9098610522), double(0.651633524), double(0.7177318024), double(-0.5528685241), double(0.5882467118), double(0.6593778956),
	double(0.9371027648), double(-0.7168658839), double(-0.4757737632), double(0.7648291307), double(0.7503650398), double(0.1705063456), double(-0.8071558121), double(-0.9951433815), double(-0.8253280792), double(-0.7982502628), double(0.9352738503), double(0.8582254747), double(-0.3465310238), double(0.65000842), double(-0.6697422351), double(0.7441962291),
	double(-0.9533555), double(0.5801940659), double(-0.9992862963), double(-0.659820211), double(0.2575848092), double(0.881588113), double(-0.9004043022), double(-0.7050172826), double(0.369126382), double(-0.02265088836), double(0.5568217228), double(-0.3316515286), double(0.991098079), double(-0.863212164), double(-0.9285531277), double(0.1695539323),
	double(-0.672402505), double(-0.001928841934), double(0.9767452145), double(-0.8767960349), double(0.9103515037), double(-0.7648324016), double(0.2706960452), double(-0.9830446035), double(0.8150341657), double(-0.9999013716), double(-0.8985605806), double(0.8533360801), double(0.8491442537), double(-0.2242541966), double(-0.1379635899), double(-0.4145572694),
	double(0.1308227633), double(0.6140555916), double(0.9417041303), double(-0.336705587), double(-0.6254387508), double(0.4631060578), double(-0.7578342456), double(-0.8172955655), double(-0.9959529228), double(-0.9760151351), double(0.2348380732), double(-0.9983612848), double(0.5856025746), double(-0.9400538266), double(-0.7639875669), double(0.6244544645),
	double(0.04604054566), double(0.5888424828), double(0.7708490978), double(-0.8114182882), double(0.9786766212), double(-0.9984329822), double(0.09125496582), double(-0.4492438803), double(-0.3636982357), double(0.9443405575), double(-0.9476254645), double(-0.6818676535), double(-0.6113610831), double(0.9754070948), double(-0.0938108173), double(-0.7029540015),
	double(-0.6085691109), double(-0.08718862881), double(-0.237381926), double(0.2913423132), double(0.9614872426), double(0.8836361266), double(-0.2749974196), double(-0.8108932717), double(-0.8913607575), double(0.129255541), double(-0.3342637104), double(-0.1921249337), double(-0.7566302845), double(-0.9563164339), double(-0.9744358146), double(0.9836522982),
	double(-0.2863615732), double(0.8337016872), double(0.3683701937), double(0.7657119102), double(-0.02312427772), double(0.8875600535), double(0.976642191), double(0.9374176384), double(0.9515313457), double(-0.7786361937), double(-0.4538302125), double(-0.7685604874), double(-0.8940796454), double(-0.8512462154), double(0.5446696133), double(0.9207601495),
	double(-0.9893091197), double(-0.9998680229), double(0.5617309299), double(-0.8277411985), double(0.545636467), double(0.1690223212), double(-0.5079295433), double(0.7685069899), double(-0.9630140787), double(0.9015219132), double(0.08905695279), double(-0.3423550559), double(-0.4661614943), double(-0.6449659371), double(0.7139388509), double(0.7472809229),
};
const double CELL_3D_X[] =
{
	double(0.3752498686), double(0.687188096), double(0.2248135212), double(0.6692006647), double(-0.4376476931), double(0.6139972552), double(0.9494563929), double(0.8065108882), double(-0.2218812853), double(0.8484661167), double(0.5551817596), double(0.2133903499), double(0.5195126593), double(-0.6440141975), double(-0.5192897331), double(-0.3697654077),
	double(-0.07927779647), double(0.4187757321), double(-0.750078731), double(0.6579554632), double(-0.6859803838), double(-0.6878407087), double(0.9490848347), double(0.5795829433), double(-0.5325976529), double(-0.1363699466), double(0.417665879), double(-0.9108236468), double(0.4438605427), double(0.819294887), double(-0.4033873915), double(-0.2817317705),
	double(0.3969665622), double(0.5323450134), double(-0.6833017297), double(0.3881436661), double(-0.7119144767), double(-0.2306979838), double(-0.9398873022), double(0.1701906676), double(-0.4261839496), double(-0.003712295499), double(-0.734675004), double(-0.3195046015), double(0.7345307424), double(0.9766246496), double(-0.02003735175), double(-0.4824156342),
	double(0.4245892007), double(0.9072427669), double(0.593346808), double(-0.8911762541), double(-0.7657571834), double(-0.5268198896), double(-0.8801903279), double(-0.6296409617), double(-0.09492481344), double(-0.4920470525), double(0.7307666154), double(-0.2514540636), double(-0.3356210347), double(-0.3522787894), double(0.87847885), double(-0.7424096346),
	double(0.5757585274), double(0.4519299338), double(0.6420368628), double(-0.1128478447), double(0.499874883), double(0.5291681739), double(-0.5098837195), double(0.5639583502), double(-0.8456386526), double(-0.9657134875), double(-0.576437342), double(-0.5666013014), double(0.5667702405), double(-0.481316582), double(0.7313389916), double(-0.3805628566),
	double(-0.6512675909), double(-0.2787156951), double(0.8648059114), double(-0.9730216276), double(-0.8335820906), double(0.2673159641), double(0.231150148), double(0.01286214638), double(0.6774953261), double(0.6542885718), double(-0.02545450161), double(0.2101238586), double(-0.5572105885), double(0.813705672), double(-0.7546026951), double(-0.2502500006),
	double(-0.9979289381), double(0.7024037039), double(0.08990874624), double(0.8170812432), double(0.4226980265), double(-0.2442153475), double(-0.9183326731), double(0.6068222411), double(0.818676691), double(-0.7236735282), double(-0.5383903295), double(-0.6269337242), double(-0.0939331121), double(0.9203878539), double(-0.7256396824), double(0.6292431149),
	double(0.4234156978), double(0.006685688024), double(-0.2598694113), double(0.6408036421), double(0.05899871622), double(0.7090281418), double(-0.5905222072), double(0.3128214264), double(-0.691925826), double(0.3634019349), double(-0.6772511147), double(-0.3204583896), double(-0.3906740409), double(-0.3342190395), double(-0.517779592), double(-0.6817711267),
	double(0.6422383105), double(0.4388482478), double(0.2968562611), double(-0.2019778353), double(0.6014865048), double(0.9519280722), double(0.3398889569), double(0.8179709354), double(0.2365522154), double(0.3262175096), double(-0.8060715954), double(-0.2068642503), double(0.6208057279), double(-0.5274282502), double(-0.3722334928), double(-0.8923412971),
	double(0.5341834201), double(-0.3663701513), double(-0.6114600319), double(0.5026307556), double(0.8396151729), double(0.9245042467), double(-0.7994843957), double(-0.5357200589), double(-0.6283359739), double(-0.61351886), double(-0.875632008), double(-0.5278879423), double(0.9087491985), double(-0.03500215466), double(-0.261365798), double(-0.579523541),
	double(-0.3765052689), double(-0.74398252), double(0.4257318052), double(-0.1214508921), double(0.8561809753), double(0.6802835104), double(-0.5452131039), double(-0.1997156478), double(0.4562348357), double(-0.811704301), double(0.67793962), double(-0.9237819106), double(0.6973511259), double(-0.5189506), double(0.5517320032), double(-0.396710831),
	double(0.5493762815), double(-0.2507853002), double(0.4788634005), double(0.387333516), double(-0.2176515694), double(0.6749832419), double(0.2148283022), double(-0.7521815872), double(0.4697000159), double(0.7890593699), double(-0.7606162952), double(0.01083397843), double(0.5254091908), double(-0.6748025877), double(0.751091524), double(0.05259056135),
	double(0.01889481232), double(-0.6037423727), double(-0.6542965129), double(0.08873301081), double(-0.6191345671), double(0.4331858488), double(-0.3858351946), double(-0.1429059747), double(0.4118221036), double(-0.6247153214), double(-0.611423014), double(0.5542939606), double(-0.9432768808), double(-0.4567870451), double(-0.7349133547), double(0.399304489),
	double(-0.7474927672), double(0.02589419753), double(0.783915821), double(0.6138668752), double(0.4276376047), double(-0.4347886353), double(0.02947841302), double(-0.833742746), double(0.3817221742), double(-0.8743368359), double(-0.3823443796), double(-0.6829243811), double(-0.3681903049), double(-0.367626833), double(-0.434583373), double(0.235891995),
	double(-0.6874880269), double(-0.5115661773), double(-0.5534962601), double(0.5632777056), double(0.686191532), double(-0.05095871588), double(-0.06865785057), double(-0.5975288531), double(-0.6429790056), double(-0.3729361548), double(0.2237917666), double(0.6046773225), double(-0.5041542295), double(-0.03972191174), double(0.7028828406), double(-0.5560856498),
	double(0.5898328456), double(-0.9308076766), double(0.4617069864), double(0.3190983137), double(0.9116567753), double(-0.45029554), double(0.3346334459), double(0.8525005645), double(0.2528483381), double(-0.8306630147), double(-0.6880390622), double(0.7448684026), double(-0.1963355843), double(-0.5900257974), double(0.9097057294), double(-0.2509196808),
};
const double CELL_3D_Y[] =
{
	double(-0.6760585049), double(-0.09136176499), double(0.1681325679), double(-0.6688468686), double(-0.4822753902), double(-0.7891068824), double(-0.1877509944), double(0.548470914), double(-0.463339443), double(-0.4050542082), double(0.3218158513), double(0.2546493823), double(-0.3753271935), double(0.4745384887), double(0.481254652), double(-0.8934416489),
	double(-0.6737085076), double(0.7469917228), double(0.3826230411), double(0.6751013678), double(-0.7248119515), double(-0.3224276742), double(-0.02076190936), double(-0.6404268166), double(-0.5292028444), double(0.7151414636), double(-0.6144655059), double(-0.369912124), double(0.6942067212), double(-0.4481558248), double(-0.6366894559), double(0.5956568471),
	double(0.564274539), double(0.7145584688), double(0.6871918316), double(0.5657918509), double(-0.6275978114), double(0.4146983062), double(0.2638993789), double(-0.792633138), double(0.5706133514), double(0.8606546462), double(0.6490900316), double(-0.8242699196), double(0.6765819124), double(0.1959534069), double(-0.8426769757), double(-0.5917672797),
	double(0.7517364266), double(0.03252559226), double(0.0883617105), double(0.4475064813), double(-0.1418643552), double(0.7343428473), double(0.3870192548), double(-0.7716703522), double(0.4839898327), double(0.7437439055), double(-0.5989573348), double(-0.8357068955), double(0.6086049038), double(0.9194627258), double(0.4718297238), double(-0.2650335884),
	double(-0.6470352599), double(-0.5555181303), double(0.1222351235), double(0.7802044684), double(-0.8636947022), double(-0.2341352163), double(0.683030874), double(-0.5005858287), double(0.2334616211), double(0.2576877608), double(0.6666816727), double(-0.7663996863), double(0.794201982), double(0.6189308788), double(0.6071033261), double(-0.4206058253),
	double(-0.3957336915), double(-0.8170257484), double(-0.1043240417), double(0.0002167596213), double(0.1816339018), double(-0.6838094939), double(-0.2495341969), double(-0.7116756954), double(-0.03361673621), double(-0.3350836431), double(0.2137186039), double(0.2557996786), double(0.7490117093), double(0.4942936549), double(-0.352686853), double(-0.3952445435),
	double(-0.0459964767), double(-0.7115787471), double(0.08022899756), double(0.5362268157), double(-0.8258613686), double(0.1114171723), double(0.3882823051), double(-0.7915404457), double(0.3250957662), double(0.6401346464), double(-0.2662724517), double(-0.6727907114), double(-0.994730818), double(-0.3596358977), double(0.2344610069), double(-0.6645215546),
	double(-0.7107590611), double(-0.4646617327), double(0.6717191355), double(0.5101893498), double(0.1185768238), double(0.236005093), double(-0.7811024061), double(0.5089325193), double(0.6073187658), double(-0.7930732557), double(-0.6822767155), double(0.3201532885), double(0.7545302807), double(0.1072664448), double(0.6784033173), double(-0.6595924967),
	double(0.7276509498), double(0.5586689436), double(-0.6498636788), double(0.6789333174), double(0.7105966551), double(-0.2872214155), double(0.496746217), double(-0.3880337977), double(0.7324070604), double(-0.9326634749), double(-0.5867839255), double(0.8003043651), double(-0.1631882481), double(-0.6796374681), double(-0.8066678503), double(0.4238177418),
	double(0.7715863549), double(0.5455367347), double(-0.03205115397), double(-0.6005545066), double(-0.5423640002), double(0.3569205906), double(-0.582071752), double(0.6407354361), double(0.7777142984), double(-0.09956428618), double(0.1100002681), double(0.8136349123), double(0.2923431904), double(0.9735794425), double(0.8324974864), double(-0.6179617717),
	double(-0.9248386523), double(-0.6448780771), double(-0.5274402761), double(-0.7862170565), double(0.2682099744), double(-0.5848777694), double(-0.6364561467), double(-0.7167402514), double(-0.8677012494), double(0.4205286707), double(-0.7007832749), double(0.243272451), double(-0.1899846085), double(-0.6146124977), double(-0.8093357692), double(-0.03545096987),
	double(-0.7191590868), double(0.7478645848), double(0.3623517328), double(0.8436992512), double(-0.2445711729), double(0.6897356637), double(-0.1708070787), double(0.4639272368), double(-0.7917186656), double(0.02980025428), double(0.6334156172), double(-0.9815544807), double(-0.2307217304), double(0.1080823318), double(0.5167601798), double(-0.845120016),
	double(0.441572562), double(0.5876789172), double(-0.6365908737), double(0.68350166), double(0.5849723959), double(0.1164114357), double(-0.7379813884), double(-0.9613237178), double(-0.9071943084), double(-0.7682111105), double(0.639074459), double(-0.619358298), double(0.2807257131), double(-0.01800868791), double(0.3776607289), double(0.7207567823),
	double(0.5536661486), double(-0.9974053117), double(-0.02047200006), double(-0.6739453804), double(-0.5607471297), double(0.8815553192), double(0.8275977415), double(0.3928902456), double(0.550991396), double(0.4247623676), double(-0.3436948871), double(-0.3653537677), double(0.3181702902), double(-0.6067173171), double(-0.8984128477), double(0.4220839766),
	double(0.7238407199), double(-0.7766913695), double(0.6460037842), double(0.2544775664), double(0.6488840578), double(0.805016833), double(-0.9183807036), double(0.4144046357), double(0.270587208), double(-0.8813684494), double(0.6985971877), double(-0.7795603017), double(-0.8624480731), double(0.5532697017), double(0.711179521), double(-0.7798160574),
	double(0.5225859041), double(0.1261859368), double(0.3398033582), double(-0.7472173667), double(-0.4032647119), double(-0.4246578154), double(0.8481212377), double(-0.2144838537), double(0.3431714491), double(0.5310188231), double(0.6682978632), double(0.3110433206), double(0.9263293599), double(-0.6155600569), double(0.07169784399), double(0.8985888773),
};
const double CELL_3D_Z[] =
{
	double(-0.6341391283), double(-0.7207118346), double(0.9597866014), double(0.3237504235), double(-0.7588642466), double(-0.01782410481), double(0.2515593809), double(0.2207257205), double(-0.8579541106), double(0.3406410681), double(0.7669470462), double(-0.9431957648), double(0.7676171537), double(-0.6000491115), double(-0.7062096948), double(0.2550207115),
	double(0.7347325213), double(0.5163625202), double(-0.5394270162), double(0.3336656285), double(-0.0638635111), double(-0.6503195787), double(0.3143356798), double(-0.5039217245), double(0.6605180464), double(-0.6855479011), double(-0.6693185756), double(0.1832083647), double(-0.5666258437), double(0.3576482138), double(-0.6571949095), double(-0.7522101635),
	double(-0.7238865886), double(0.4538887323), double(0.2467106257), double(0.7274778869), double(0.3151170655), double(-0.8802293764), double(-0.2167232729), double(0.5854637865), double(0.7019741052), double(0.5091756071), double(0.1973189533), double(0.46743546), double(0.05197599597), double(0.088354718), double(0.5380464843), double(-0.6458224544),
	double(-0.5045952393), double(0.419347884), double(0.8000823542), double(-0.07445020656), double(-0.6272881641), double(-0.428020311), double(-0.2747382083), double(-0.08987283726), double(0.8699098354), double(0.4524761885), double(-0.3274603257), double(0.4882262167), double(-0.7189983256), double(0.1746079907), double(0.0751772698), double(-0.6152927202),
	double(0.4998474673), double(-0.6979677227), double(0.7568667263), double(-0.6152612058), double(0.06447140991), double(-0.8155744872), double(-0.5229602449), double(0.6567836838), double(-0.4799905631), double(0.03153534591), double(0.4724992466), double(-0.3026458097), double(-0.2191225827), double(-0.620692287), double(0.3107552588), double(0.8235670294),
	double(0.6474915988), double(-0.5047637941), double(0.4911488878), double(-0.2307138167), double(-0.5216800015), double(0.6789305939), double(0.9403734863), double(0.702390397), double(0.7347584625), double(0.6779567958), double(0.9765635805), double(-0.9436177661), double(-0.358465925), double(-0.3058706624), double(0.5533414464), double(-0.8838306897),
	double(0.04496841812), double(0.01687374963), double(-0.9927133148), double(-0.211752318), double(0.3732015249), double(0.9632990593), double(-0.07682417004), double(-0.07232213047), double(0.4733721775), double(0.2579229713), double(0.7995216286), double(0.3928189967), double(0.04107517667), double(0.1534542912), double(0.6468965045), double(0.4030684878),
	double(-0.5617300988), double(-0.885463029), double(0.693729985), double(-0.5736527866), double(-0.9911905409), double(-0.66451538), double(0.2028855685), double(0.8019541421), double(-0.3903877149), double(-0.4888495114), double(-0.2753714057), double(-0.8915202143), double(0.5273119089), double(0.9363714773), double(-0.5212228249), double(-0.31642672),
	double(0.2409440761), double(-0.703776404), double(-0.6996810411), double(-0.7058714505), double(-0.3650566783), double(0.1064744278), double(0.7985729102), double(0.424680257), double(-0.6384535592), double(0.1540161646), double(-0.07702731943), double(-0.5627789132), double(-0.7667919169), double(-0.509815999), double(0.4590525092), double(0.1552595611),
	double(0.345402042), double(0.7537656024), double(0.7906259247), double(-0.6218493452), double(0.02979350071), double(-0.1337893489), double(-0.1483818606), double(0.549965562), double(0.01882482408), double(-0.7833783002), double(0.4702855809), double(0.2435827372), double(0.2978428332), double(0.2256499906), double(0.4885036897), double(0.5312962584),
	double(0.05401156992), double(0.1749922158), double(-0.7352273018), double(0.6058980284), double(0.4416079111), double(0.4417378638), double(0.5455879807), double(-0.6681295324), double(0.1973431441), double(0.4053292055), double(0.2220375492), double(0.2957118467), double(0.6910913512), double(0.5940890106), double(-0.2014135283), double(-0.9172588213),
	double(-0.4254361401), double(-0.6146586825), double(-0.7996193253), double(-0.3716777111), double(-0.9448876842), double(-0.2620349924), double(0.9615995749), double(-0.4679683524), double(0.3905937144), double(0.613593722), double(0.1422937358), double(0.1908754211), double(0.8189704912), double(-0.7300408736), double(-0.4108776451), double(-0.5319834504),
	double(-0.8970265651), double(-0.5386359045), double(0.4082255906), double(0.7245356676), double(0.5239080873), double(-0.8937552226), double(-0.553637673), double(0.2354455182), double(-0.0860293075), double(-0.1399373318), double(-0.4666323327), double(0.5560157407), double(0.1772619533), double(-0.8893937725), double(-0.5632714576), double(-0.5666264959),
	double(-0.3670263736), double(-0.06717242579), double(0.6205295181), double(-0.4110536264), double(0.7090054553), double(0.183899597), double(-0.5605470555), double(0.3879565548), double(0.7420893903), double(-0.2347595118), double(-0.8577217497), double(0.6325590203), double(-0.8736152276), double(0.7048011129), double(-0.06317948268), double(0.8753285574),
	double(-0.05843650473), double(-0.3674922622), double(-0.5256624401), double(0.7861039337), double(0.3287714416), double(0.5910593099), double(-0.3896960134), double(0.6864605361), double(0.7164918431), double(-0.290014277), double(-0.6796169617), double(0.1632515592), double(0.04485347486), double(0.8320545697), double(0.01339408056), double(-0.2874989857),
	double(0.615630723), double(0.3430367014), double(0.8193658136), double(-0.5829600957), double(0.07911697781), double(0.7854296063), double(-0.4107442306), double(0.4766964066), double(-0.9045999527), double(-0.1673856787), double(0.2828077348), double(-0.5902737632), double(-0.321506229), double(-0.5224513133), double(-0.4090169985), double(-0.3599685311),
};

static double FastFloor(double f) { return (f >= 0 ? f : f - 1); }
static int FastRound(double f) { return (f >= 0) ? (int)(f + double(0.5)) : (int)(f - double(0.5)); }
static int FastAbs(int i) { return abs(i); }
static double FastAbs(double f) { return fabs(f); }
static double Lerp(double a, double b, double t) { return a + t * (b - a); }
static double InterpHermiteFunc(double t) { return t*t*(3 - 2 * t); }
static double InterpQuinticFunc(double t) { return t*t*t*(t*(t * 6 - 15) + 10); }
static double CubicLerp(double a, double b, double c, double d, double t)
{
	double p = (d - c) - (a - b);
	return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
}

void FastNoise::SetSeed(int seed)
{
	m_seed = seed;

	std::mt19937_64 gen(seed);

	for (int i = 0; i < 256; i++)
		m_perm[i] = i;

	for (int j = 0; j < 256; j++)
	{
        int rng = (int)(gen() % (256 - j));
		int k = rng + j;
		int l = m_perm[j];
		m_perm[j] = m_perm[j + 256] = m_perm[k];
		m_perm[k] = l;
		m_perm12[j] = m_perm12[j + 256] = m_perm[j] % 12;
	}
}

void FastNoise::CalculateFractalBounding()
{
	double amp = m_gain;
	double ampFractal = 1.0f;
	for (int i = 1; i < m_octaves; i++)
	{
		ampFractal += amp;
		amp *= m_gain;
	}
	m_fractalBounding = 1.0f / ampFractal;
}

void FastNoise::SetCellularDistance2Indices(int cellularDistanceIndex0, int cellularDistanceIndex1)
{
	m_cellularDistanceIndex0 = std::min(cellularDistanceIndex0, cellularDistanceIndex1);
	m_cellularDistanceIndex1 = std::max(cellularDistanceIndex0, cellularDistanceIndex1);

	m_cellularDistanceIndex0 = std::min(std::max(m_cellularDistanceIndex0, 0), FN_CELLULAR_INDEX_MAX);
	m_cellularDistanceIndex1 = std::min(std::max(m_cellularDistanceIndex1, 0), FN_CELLULAR_INDEX_MAX);
}

void FastNoise::GetCellularDistance2Indices(int& cellularDistanceIndex0, int& cellularDistanceIndex1) const
{
	cellularDistanceIndex0 = m_cellularDistanceIndex0;
	cellularDistanceIndex1 = m_cellularDistanceIndex1;
}

unsigned char FastNoise::Index2D_12(unsigned char offset, int x, int y) const
{
	return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + offset]];
}
unsigned char FastNoise::Index3D_12(unsigned char offset, int x, int y, int z) const
{
	return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
}
unsigned char FastNoise::Index4D_32(unsigned char offset, int x, int y, int z, int w) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + m_perm[(w & 0xff) + offset]]]] & 31;
}
unsigned char FastNoise::Index2D_256(unsigned char offset, int x, int y) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + offset]];
}
unsigned char FastNoise::Index3D_256(unsigned char offset, int x, int y, int z) const
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
}

// Hashing
#define X_PRIME 1619
#define Y_PRIME 31337
#define Z_PRIME 6971
#define W_PRIME 1013

static double ValCoord2D(int seed, int x, int y)
{
	int n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;

	return (n * n * n * 60493) / double(2147483648);
}
static double ValCoord3D(int seed, int x, int y, int z)
{
	int n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;
	n ^= Z_PRIME * z;

	return (n * n * n * 60493) / double(2147483648);
}
static double ValCoord4D(int seed, int x, int y, int z, int w)
{
	int n = seed;
	n ^= X_PRIME * x;
	n ^= Y_PRIME * y;
	n ^= Z_PRIME * z;
	n ^= W_PRIME * w;

	return (n * n * n * 60493) / double(2147483648);
}

double FastNoise::ValCoord2DFast(unsigned char offset, int x, int y) const
{
	return VAL_LUT[Index2D_256(offset, x, y)];
}
double FastNoise::ValCoord3DFast(unsigned char offset, int x, int y, int z) const
{
	return VAL_LUT[Index3D_256(offset, x, y, z)];
}

double FastNoise::GradCoord2D(unsigned char offset, int x, int y, double xd, double yd) const
{
	unsigned char lutPos = Index2D_12(offset, x, y);

	return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos];
}
double FastNoise::GradCoord3D(unsigned char offset, int x, int y, int z, double xd, double yd, double zd) const
{
	unsigned char lutPos = Index3D_12(offset, x, y, z);

	return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos] + zd*GRAD_Z[lutPos];
}
double FastNoise::GradCoord4D(unsigned char offset, int x, int y, int z, int w, double xd, double yd, double zd, double wd) const
{
	unsigned char lutPos = Index4D_32(offset, x, y, z, w) << 2;

	return xd*GRAD_4D[lutPos] + yd*GRAD_4D[lutPos + 1] + zd*GRAD_4D[lutPos + 2] + wd*GRAD_4D[lutPos + 3];
}

double FastNoise::GetNoise(double x, double y, double z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch (m_noiseType)
	{
	case Perlin:
		return SinglePerlin(0, x, y, z);
	case PerlinFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SinglePerlinFractalFBM(x, y, z);
		case Billow:
			return SinglePerlinFractalBillow(x, y, z);
		case RigidMulti:
			return SinglePerlinFractalRigidMulti(x, y, z);
		default:
			return 0;
		}
	case Simplex:
		return SingleSimplex(0, x, y, z);
	case SimplexFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SingleSimplexFractalFBM(x, y, z);
		case Billow:
			return SingleSimplexFractalBillow(x, y, z);
		case RigidMulti:
			return SingleSimplexFractalRigidMulti(x, y, z);
		default:
			return 0;
		}
	default:
		return 0;
	}
}

double FastNoise::GetNoise(double x, double y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch (m_noiseType)
	{
	case Perlin:
		return SinglePerlin(0, x, y);
	case PerlinFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SinglePerlinFractalFBM(x, y);
		case Billow:
			return SinglePerlinFractalBillow(x, y);
		case RigidMulti:
			return SinglePerlinFractalRigidMulti(x, y);
		}
	case Simplex:
		return SingleSimplex(0, x, y);
	case SimplexFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SingleSimplexFractalFBM(x, y);
		case Billow:
			return SingleSimplexFractalBillow(x, y);
		case RigidMulti:
			return SingleSimplexFractalRigidMulti(x, y);
		}
	}
	return 0;
}

// Perlin Noise
double FastNoise::GetPerlinFractal(double x, double y, double z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SinglePerlinFractalFBM(x, y, z);
	case Billow:
		return SinglePerlinFractalBillow(x, y, z);
	case RigidMulti:
		return SinglePerlinFractalRigidMulti(x, y, z);
	default:
		return 0;
	}
}

double FastNoise::SinglePerlinFractalFBM(double x, double y, double z) const
{
	double sum = SinglePerlin(m_perm[0], x, y, z);
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SinglePerlin(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

double FastNoise::SinglePerlinFractalBillow(double x, double y, double z) const
{
	double sum = FastAbs(SinglePerlin(m_perm[0], x, y, z)) * 2 - 1;
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SinglePerlin(m_perm[i], x, y, z)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

double FastNoise::SinglePerlinFractalRigidMulti(double x, double y, double z) const
{
	double sum = 1 - FastAbs(SinglePerlin(m_perm[0], x, y, z));
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SinglePerlin(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

double FastNoise::GetPerlin(double x, double y, double z) const
{
	return SinglePerlin(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

double FastNoise::SinglePerlin(unsigned char offset, double x, double y, double z) const
{
	int x0 = (int) FastFloor(x);
	int y0 = (int) FastFloor(y);
	int z0 = (int) FastFloor(z);

	double xd0 = (double) (x - x0);
	double yd0 = (double) (y - y0);
	double zd0 = (double) (z - z0);

	double xs, ys, zs;
	switch (m_interp)
	{
	case Linear:
		xs = xd0;
		ys = yd0;
		zs = zd0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(xd0);
		ys = InterpHermiteFunc(yd0);
		zs = InterpHermiteFunc(zd0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(xd0);
		ys = InterpQuinticFunc(yd0);
		zs = InterpQuinticFunc(zd0);
		break;
	}

	double xf00 = Lerp(GradCoord3D(offset, x0, y0, z0, xd0, yd0, zd0), GradCoord3D(offset, x0 + 1, y0, z0, xd0 - 1, yd0, zd0), xs);
	double xf10 = Lerp(GradCoord3D(offset, x0, y0 + 1, z0, xd0, yd0 - 1, zd0), GradCoord3D(offset, x0 + 1, y0 + 1, z0, xd0 - 1, yd0 - 1, zd0), xs);
	double xf01 = Lerp(GradCoord3D(offset, x0, y0, z0 + 1, xd0, yd0, zd0 - 1), GradCoord3D(offset, x0 + 1, y0, z0 + 1, xd0 - 1, yd0, zd0 - 1), xs);
	double xf11 = Lerp(GradCoord3D(offset, x0, y0 + 1, z0 + 1, xd0, yd0 - 1, zd0 - 1), GradCoord3D(offset, x0 + 1, y0 + 1, z0 + 1, xd0 - 1, yd0 - 1, zd0 - 1), xs);

	double yf0 = Lerp(xf00, xf10, ys);
	double yf1 = Lerp(xf01, xf11, ys);

	return Lerp(yf0, yf1, zs);
}

double FastNoise::GetPerlinFractal(double x, double y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SinglePerlinFractalFBM(x, y);
	case Billow:
		return SinglePerlinFractalBillow(x, y);
	case RigidMulti:
		return SinglePerlinFractalRigidMulti(x, y);
	default:
		return 0;
	}
}

double FastNoise::SinglePerlinFractalFBM(double x, double y) const
{
	double sum = SinglePerlin(m_perm[0], x, y);
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SinglePerlin(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

double FastNoise::SinglePerlinFractalBillow(double x, double y) const
{
	double sum = FastAbs(SinglePerlin(m_perm[0], x, y)) * 2 - 1;
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SinglePerlin(m_perm[i], x, y)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

double FastNoise::SinglePerlinFractalRigidMulti(double x, double y) const
{
	double sum = 1 - FastAbs(SinglePerlin(m_perm[0], x, y));
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SinglePerlin(m_perm[i], x, y))) * amp;
	}

	return sum;
}

double FastNoise::GetPerlin(double x, double y) const
{
	return SinglePerlin(0, x * m_frequency, y * m_frequency);
}

double FastNoise::SinglePerlin(unsigned char offset, double x, double y) const
{
	int x0 = (int) FastFloor(x);
	int y0 = (int) FastFloor(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	double xs, ys;
	switch (m_interp)
	{
	case Linear:
		xs = x - (double)x0;
		ys = y - (double)y0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (double)x0);
		ys = InterpHermiteFunc(y - (double)y0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (double)x0);
		ys = InterpQuinticFunc(y - (double)y0);
		break;
	}

	double xd0 = x - (double)x0;
	double yd0 = y - (double)y0;
	double xd1 = xd0 - 1;
	double yd1 = yd0 - 1;

	double xf0 = Lerp(GradCoord2D(offset, x0, y0, xd0, yd0), GradCoord2D(offset, x1, y0, xd1, yd0), xs);
	double xf1 = Lerp(GradCoord2D(offset, x0, y1, xd0, yd1), GradCoord2D(offset, x1, y1, xd1, yd1), xs);

	return Lerp(xf0, xf1, ys);
}

// Simplex Noise

double FastNoise::GetSimplexFractal(double x, double y, double z) const
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleSimplexFractalFBM(x, y, z);
	case Billow:
		return SingleSimplexFractalBillow(x, y, z);
	case RigidMulti:
		return SingleSimplexFractalRigidMulti(x, y, z);
	default:
		return 0;
	}
}

double FastNoise::SingleSimplexFractalFBM(double x, double y, double z) const
{
	double sum = SingleSimplex(m_perm[0], x, y, z);
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SingleSimplex(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

double FastNoise::SingleSimplexFractalBillow(double x, double y, double z) const
{
	double sum = FastAbs(SingleSimplex(m_perm[0], x, y, z)) * 2 - 1;
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleSimplex(m_perm[i], x, y, z)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

double FastNoise::SingleSimplexFractalRigidMulti(double x, double y, double z) const
{
	double sum = 1 - FastAbs(SingleSimplex(m_perm[0], x, y, z));
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SingleSimplex(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

double FastNoise::GetSimplex(double x, double y, double z) const
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

static const double F3 = 1 / double(3);
static const double G3 = 1 / double(6);

double FastNoise::SingleSimplex(unsigned char offset, double x, double y, double z) const
{
	double t = (x + y + z) * F3;
	int i = (int) FastFloor(x + t);
	int j = (int) FastFloor(y + t);
	int k = (int) FastFloor(z + t);

	t = (i + j + k) * G3;
	double X0 = i - t;
	double Y0 = j - t;
	double Z0 = k - t;

	double x0 = x - X0;
	double y0 = y - Y0;
	double z0 = z - Z0;

	int i1, j1, k1;
	int i2, j2, k2;

	if (x0 >= y0)
	{
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
		else if (x0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;
		}
		else // x0 < z0
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;
		}
	}
	else // x0 < y0
	{
		if (y0 < z0)
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;
		}
		else if (x0 < z0)
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
		}
		else // x0 >= z0
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
	}

	double x1 = x0 - i1 + G3;
	double y1 = y0 - j1 + G3;
	double z1 = z0 - k1 + G3;
	double x2 = x0 - i2 + 2*G3;
	double y2 = y0 - j2 + 2*G3;
	double z2 = z0 - k2 + 2*G3;
	double x3 = x0 - 1 + 3*G3;
	double y3 = y0 - 1 + 3*G3;
	double z3 = z0 - 1 + 3*G3;

	double n0, n1, n2, n3;

	t = double(0.6) - x0*x0 - y0*y0 - z0*z0;
	if (t < 0) n0 = 0;
	else
	{
		t *= t;
		n0 = t*t*GradCoord3D(offset, i, j, k, x0, y0, z0);
	}

	t = double(0.6) - x1*x1 - y1*y1 - z1*z1;
	if (t < 0) n1 = 0;
	else
	{
		t *= t;
		n1 = t*t*GradCoord3D(offset, i + i1, j + j1, k + k1, x1, y1, z1);
	}

	t = double(0.6) - x2*x2 - y2*y2 - z2*z2;
	if (t < 0) n2 = 0;
	else
	{
		t *= t;
		n2 = t*t*GradCoord3D(offset, i + i2, j + j2, k + k2, x2, y2, z2);
	}

	t = double(0.6) - x3*x3 - y3*y3 - z3*z3;
	if (t < 0) n3 = 0;
	else
	{
		t *= t;
		n3 = t*t*GradCoord3D(offset, i + 1, j + 1, k + 1, x3, y3, z3);
	}

	return 32 * (n0 + n1 + n2 + n3);
}

double FastNoise::GetSimplexFractal(double x, double y) const
{
	x *= m_frequency;
	y *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleSimplexFractalFBM(x, y);
	case Billow:
		return SingleSimplexFractalBillow(x, y);
	case RigidMulti:
		return SingleSimplexFractalRigidMulti(x, y);
	default:
		return 0;
	}
}

double FastNoise::SingleSimplexFractalFBM(double x, double y) const
{
	double sum = SingleSimplex(m_perm[0], x, y);
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SingleSimplex(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

double FastNoise::SingleSimplexFractalBillow(double x, double y) const
{
	double sum = FastAbs(SingleSimplex(m_perm[0], x, y)) * 2 - 1;
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleSimplex(m_perm[i], x, y)) * 2 - 1) * amp;
	}

	return sum * m_fractalBounding;
}

double FastNoise::SingleSimplexFractalRigidMulti(double x, double y) const
{
	double sum = 1 - FastAbs(SingleSimplex(m_perm[0], x, y));
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1 - FastAbs(SingleSimplex(m_perm[i], x, y))) * amp;
	}

	return sum;
}

double FastNoise::SingleSimplexFractalBlend(double x, double y) const
{
	double sum = SingleSimplex(m_perm[0], x, y);
	double amp = 1;
	int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum *= SingleSimplex(m_perm[i], x, y) * amp + 1;
	}

	return sum * m_fractalBounding;
}

double FastNoise::GetSimplex(double x, double y) const
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency);
}

//static const double F2 = 1 / double(2);
//static const double G2 = 1 / double(4);

static const double SQRT3 = double(1.7320508075688772935274463415059);
static const double F2 = double(0.5) * (SQRT3 - double(1.0));
static const double G2 = (double(3.0) - SQRT3) / double(6.0);

double FastNoise::SingleSimplex(unsigned char offset, double x, double y) const
{
	double t = (x + y) * F2;
	int i = (int) FastFloor(x + t);
	int j = (int) FastFloor(y + t);

	t = (i + j) * G2;
	double X0 = i - t;
	double Y0 = j - t;

	double x0 = x - X0;
	double y0 = y - Y0;

	int i1, j1;
	if (x0 > y0)
	{
		i1 = 1; j1 = 0;
	}
	else
	{
		i1 = 0; j1 = 1;
	}

	double x1 = x0 - (double)i1 + G2;
	double y1 = y0 - (double)j1 + G2;
	double x2 = x0 - 1 + 2*G2;
	double y2 = y0 - 1 + 2*G2;

	double n0, n1, n2;

	t = double(0.5) - x0*x0 - y0*y0;
	if (t < 0) n0 = 0;
	else
	{
		t *= t;
		n0 = t * t * GradCoord2D(offset, i, j, x0, y0);
	}

	t = double(0.5) - x1*x1 - y1*y1;
	if (t < 0) n1 = 0;
	else
	{
		t *= t;
		n1 = t*t*GradCoord2D(offset, i + i1, j + j1, x1, y1);
	}

	t = double(0.5) - x2*x2 - y2*y2;
	if (t < 0) n2 = 0;
	else
	{
		t *= t;
		n2 = t*t*GradCoord2D(offset, i + 1, j + 1, x2, y2);
	}

	return 70 * (n0 + n1 + n2);
}

double FastNoise::GetSimplex(double x, double y, double z, double w) const
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency, z * m_frequency, w * m_frequency);
}

static const double F4 = (sqrt(double(5)) - 1) / 4;
static const double G4 = (5 - sqrt(double(5))) / 20;

double FastNoise::SingleSimplex(unsigned char offset, double x, double y, double z, double w) const
{
	double n0, n1, n2, n3, n4;
	double t = (x + y + z + w) * F4;
	int i = (int) FastFloor(x + t);
	int j = (int) FastFloor(y + t);
	int k = (int) FastFloor(z + t);
	int l = (int) FastFloor(w + t);
	t = (i + j + k + l) * G4;
	double X0 = i - t;
	double Y0 = j - t;
	double Z0 = k - t;
	double W0 = l - t;
	double x0 = x - X0;
	double y0 = y - Y0;
	double z0 = z - Z0;
	double w0 = w - W0;

	int rankx = 0;
	int ranky = 0;
	int rankz = 0;
	int rankw = 0;

	if (x0 > y0) rankx++; else ranky++;
	if (x0 > z0) rankx++; else rankz++;
	if (x0 > w0) rankx++; else rankw++;
	if (y0 > z0) ranky++; else rankz++;
	if (y0 > w0) ranky++; else rankw++;
	if (z0 > w0) rankz++; else rankw++;

	int i1 = rankx >= 3 ? 1 : 0;
	int j1 = ranky >= 3 ? 1 : 0;
	int k1 = rankz >= 3 ? 1 : 0;
	int l1 = rankw >= 3 ? 1 : 0;

	int i2 = rankx >= 2 ? 1 : 0;
	int j2 = ranky >= 2 ? 1 : 0;
	int k2 = rankz >= 2 ? 1 : 0;
	int l2 = rankw >= 2 ? 1 : 0;

	int i3 = rankx >= 1 ? 1 : 0;
	int j3 = ranky >= 1 ? 1 : 0;
	int k3 = rankz >= 1 ? 1 : 0;
	int l3 = rankw >= 1 ? 1 : 0;

	double x1 = x0 - i1 + G4;
	double y1 = y0 - j1 + G4;
	double z1 = z0 - k1 + G4;
	double w1 = w0 - l1 + G4;
	double x2 = x0 - i2 + 2*G4;
	double y2 = y0 - j2 + 2*G4;
	double z2 = z0 - k2 + 2*G4;
	double w2 = w0 - l2 + 2*G4;
	double x3 = x0 - i3 + 3*G4;
	double y3 = y0 - j3 + 3*G4;
	double z3 = z0 - k3 + 3*G4;
	double w3 = w0 - l3 + 3*G4;
	double x4 = x0 - 1 + 4*G4;
	double y4 = y0 - 1 + 4*G4;
	double z4 = z0 - 1 + 4*G4;
	double w4 = w0 - 1 + 4*G4;

	t = double(0.6) - x0*x0 - y0*y0 - z0*z0 - w0*w0;
	if (t < 0) n0 = 0;
	else {
		t *= t;
		n0 = t * t * GradCoord4D(offset, i, j, k, l, x0, y0, z0, w0);
	}
	t = double(0.6) - x1*x1 - y1*y1 - z1*z1 - w1*w1;
	if (t < 0) n1 = 0;
	else {
		t *= t;
		n1 = t * t * GradCoord4D(offset, i + i1, j + j1, k + k1, l + l1, x1, y1, z1, w1);
	}
	t = double(0.6) - x2*x2 - y2*y2 - z2*z2 - w2*w2;
	if (t < 0) n2 = 0;
	else {
		t *= t;
		n2 = t * t * GradCoord4D(offset, i + i2, j + j2, k + k2, l + l2, x2, y2, z2, w2);
	}
	t = double(0.6) - x3*x3 - y3*y3 - z3*z3 - w3*w3;
	if (t < 0) n3 = 0;
	else {
		t *= t;
		n3 = t * t * GradCoord4D(offset, i + i3, j + j3, k + k3, l + l3, x3, y3, z3, w3);
	}
	t = double(0.6) - x4*x4 - y4*y4 - z4*z4 - w4*w4;
	if (t < 0) n4 = 0;
	else {
		t *= t;
		n4 = t * t * GradCoord4D(offset, i + 1, j + 1, k + 1, l + 1, x4, y4, z4, w4);
	}

	return 27 * (n0 + n1 + n2 + n3 + n4);
}
