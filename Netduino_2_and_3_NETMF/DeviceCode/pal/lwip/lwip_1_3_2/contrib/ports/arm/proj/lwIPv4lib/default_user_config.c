/////////////////////////////////////////////////
//      Generated file Do not Modify
/////////////////////////////////////////////////
typedef struct net_config_info
				{
				    int   imask;
					short rx_buffs;
					short tx_buffs;
					short rx_buff_datalen;
					short tx_buff_datalen;
					short buff_area;
					short buff_area_size;
					short use_dhcp;
					unsigned char mac_addr[6];
			        int ipaddr;
					int netmask;
					int gateway;
				}net_config_info;
extern struct net_config_info user_net_config_info[];

struct net_config_info user_net_config_info[]= {
                                                 {
                                                   0,
                                                    60,
                                                    40,
                                                     1600,
                                                     1548,
                                                   0,
                                                   0,
                                                     1,
											{0x00,0x00,0x00,0x00,0x00,0x00},

                                                     0x0,
                                                     0x0,
                                                     0x0,
                },
           };
int user_net_num_ifces = sizeof(user_net_config_info)/sizeof(net_config_info);
