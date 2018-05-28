from trex_astf_lib.api import *

# Only TCP SFR data is here 
# SFR UDP templates are not here yet (e.g. DNS)
# 


class Prof1():
    def __init__(self):
        pass  # tunables

    def create_profile(self):
        # ip generator
        ip_gen_c = ASTFIPGenDist(ip_range=["16.0.0.1", "16.0.0.255"], distribution="seq")
        ip_gen_s = ASTFIPGenDist(ip_range=["48.0.0.1", "48.0.255.255"], distribution="seq")
        ip_gen = ASTFIPGen(glob=ASTFIPGenGlobal(ip_offset="1.0.0.0"),
                           dist_client=ip_gen_c,
                           dist_server=ip_gen_s)

        profile = ASTFProfile(default_ip_gen=ip_gen, cap_list=[
            ASTFCapInfo(file="../avl/delay_10_http_get_0.pcap", cps=102.0,port=8080),
            ASTFCapInfo(file="../avl/delay_10_http_post_0.pcap", cps=102.0,port=8081),
            ASTFCapInfo(file="../avl/delay_10_https_0.pcap", cps=33.0),
            ASTFCapInfo(file="../avl/delay_10_http_browsing_0.pcap", cps=179.0),


            ASTFCapInfo(file="../avl/delay_10_exchange_0.pcap", cps=64.0),

            ASTFCapInfo(file="../avl/delay_10_mail_pop_0.pcap", cps=1.2),
            ASTFCapInfo(file="../avl/delay_10_mail_pop_1.pcap", cps=1.2,port=111),
            ASTFCapInfo(file="../avl/delay_10_mail_pop_2.pcap", cps=1.2,port=112),

            ASTFCapInfo(file="../avl/delay_10_oracle_0.pcap", cps=20.0),

            ASTFCapInfo(file="../avl/delay_10_rtp_160k_0.pcap", cps=0.7),
            ASTFCapInfo(file="../avl/delay_10_rtp_160k_1.pcap", cps=0.7),
            ASTFCapInfo(file="../avl/delay_10_rtp_250k_0_0.pcap", cps=0.5),
            ASTFCapInfo(file="../avl/delay_10_rtp_250k_1_0.pcap", cps=0.5),

            ASTFCapInfo(file="../avl/delay_10_smtp_0.pcap", cps=1.85),
            ASTFCapInfo(file="../avl/delay_10_smtp_1.pcap", cps=1.85,port=26),
            ASTFCapInfo(file="../avl/delay_10_smtp_2.pcap", cps=1.85,port=27),

            ASTFCapInfo(file="../avl/delay_10_video_call_rtp_0.pcap", cps=7.4),

            ASTFCapInfo(file="../avl/delay_10_citrix_0.pcap", cps=11.0),

            ASTFCapInfo(file="../avl/delay_10_dns_0.pcap", cps=498.0),

            ASTFCapInfo(file="../avl/delay_10_sip_0.pcap", cps=7.4),
            ASTFCapInfo(file="../avl/delay_10_rtsp_0.pcap", cps=1.2),

            ASTFCapInfo(file="../avl/delay_10_http_get_0.pcap", cps=102.0,port=9080),
            ASTFCapInfo(file="../avl/delay_10_http_post_0.pcap", cps=102.0,port=9081),
            ASTFCapInfo(file="../avl/delay_10_https_0.pcap", cps=933.0,port=9089),
            ASTFCapInfo(file="../avl/delay_10_http_browsing_0.pcap", cps=179.0,port=9100),


            ASTFCapInfo(file="../avl/delay_10_exchange_0.pcap", cps=64.0,port=9101),

            ASTFCapInfo(file="../avl/delay_10_mail_pop_0.pcap", cps=1.2,port=9102),
            ASTFCapInfo(file="../avl/delay_10_mail_pop_1.pcap", cps=1.2,port=9103),
            ASTFCapInfo(file="../avl/delay_10_mail_pop_2.pcap", cps=1.2,port=9104),

            ASTFCapInfo(file="../avl/delay_10_oracle_0.pcap", cps=20.0,port=9105),

            ASTFCapInfo(file="../avl/delay_10_rtp_160k_0.pcap", cps=0.7,port=9106),
            ASTFCapInfo(file="../avl/delay_10_rtp_160k_1.pcap", cps=0.7,port=9107),
            ASTFCapInfo(file="../avl/delay_10_rtp_250k_0_0.pcap", cps=0.5,port=9108),
            ASTFCapInfo(file="../avl/delay_10_rtp_250k_1_0.pcap", cps=0.5,port=9110),

            ASTFCapInfo(file="../avl/delay_10_smtp_0.pcap", cps=1.85,port=9111),
            ASTFCapInfo(file="../avl/delay_10_smtp_1.pcap", cps=1.85,port=9112),
            ASTFCapInfo(file="../avl/delay_10_smtp_2.pcap", cps=1.85,port=9113),

            ASTFCapInfo(file="../avl/delay_10_video_call_rtp_0.pcap", cps=7.4,port=9114),

            ASTFCapInfo(file="../avl/delay_10_citrix_0.pcap", cps=11.0,port=9115),

            ASTFCapInfo(file="../avl/delay_10_dns_0.pcap", cps=498.0,port=9116),

            ASTFCapInfo(file="../avl/delay_10_sip_0.pcap", cps=7.4,port=9117),
            ASTFCapInfo(file="../avl/delay_10_rtsp_0.pcap", cps=1.2,port=9118),


        ])

        return profile

    def get_profile(self, **kwargs):
        return self.create_profile()


def register():
    return Prof1()
