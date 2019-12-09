#pragma GCC diagnostic ignored "-Wdeprecated-declarations" 
#include "xsim.dir/emu_wrapper_behav/obj/sysc_interface.h"
#include <map>
#include <list>


extern "C" { std::map<unsigned int, std::pair<unsigned int, std::list < void*> > > *_glb_hdl_sc_channel_map; } 



extern "C"
    void call_HDL_SC_WRITE(unsigned int netid, int len, char* pval)
    {
        unsigned int data_type = (*_glb_hdl_sc_channel_map)[netid].first;
        std::list<void*> list_of_ch = (*_glb_hdl_sc_channel_map)[netid].second;
        switch (data_type) {
            case 0 /*int*/ : { 
                int val = *((int*)(pval));
                for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                    ((xsim_sc_channel<0,int>*)(*it))->update_from_hdl(val);
                break;
            } 
            case 1 /*bool*/ : { 
                bool val = *((bool*)(pval));
                for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                    ((xsim_sc_channel<0,bool>*)(*it))->update_from_hdl(val);
                break;
            } 
            case 2 /*sc_logic*/ : { 
                sc_dt::sc_logic val  =  (sc_dt::sc_logic_value_t) (((*((int*)(pval)+1) & 0x1) << 1) | (*((int*)(pval)) & 0x1));
                for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                    ((xsim_sc_channel<0, sc_dt::sc_logic>*)(*it))->update_from_hdl(val);
                break;
            } 
            case 3 /*sc_bv*/ : { 
                switch (len) { 
                    case 2 : { 
                        for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                            ((xsim_sc_channel<2, sc_dt::sc_bv<2> >*)(*it))->update_from_hdl(pval);
                        break;
                    }
                    case 3 : { 
                        for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                            ((xsim_sc_channel<3, sc_dt::sc_bv<3> >*)(*it))->update_from_hdl(pval);
                        break;
                    }
                    case 4 : { 
                        for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                            ((xsim_sc_channel<4, sc_dt::sc_bv<4> >*)(*it))->update_from_hdl(pval);
                        break;
                    }
                    case 6 : { 
                        for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                            ((xsim_sc_channel<6, sc_dt::sc_bv<6> >*)(*it))->update_from_hdl(pval);
                        break;
                    }
                    case 8 : { 
                        for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                            ((xsim_sc_channel<8, sc_dt::sc_bv<8> >*)(*it))->update_from_hdl(pval);
                        break;
                    }
                    case 32 : { 
                        for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                            ((xsim_sc_channel<32, sc_dt::sc_bv<32> >*)(*it))->update_from_hdl(pval);
                        break;
                    }
                    case 64 : { 
                        for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                            ((xsim_sc_channel<64, sc_dt::sc_bv<64> >*)(*it))->update_from_hdl(pval);
                        break;
                    }
                        default: 
                            std::cout << "Error, unknown bitvector length: " << len << " NOT supported " << std::endl;
                            assert(0);
                }; 
                break;
            } 
            case 5 /*bit*/ : { 
                sc_dt::sc_bit val  =  (sc_dt::sc_bit)((*((int*)(pval)) & 0x1));
                for ( std::list<void*>::iterator it = list_of_ch.begin(); it != list_of_ch.end(); ++it)
                    ((xsim_sc_channel<0, sc_dt::sc_bit>*)(*it))->update_from_hdl(val);
                break;
            } 
            default: 
                std::cout << "Error, unknown type: " << data_type << " NOT supported " << std::endl;
                assert(0);
        };
    }


extern "C"
    void implicit_HDL_SCinstatiate()
    {
        _glb_hdl_sc_channel_map = new std::map <unsigned int, std::pair<unsigned int, std::list<void*> > > ();
        emu_sim_accel_mon_0_0 *sc_inst_1 = new emu_sim_accel_mon_0_0(".emu_wrapper.emu_i.sim_accel_mon_0");
        xsim_sc_channel<6, sc_dt::sc_bv<6> > *p_1_axi_awaddr = new xsim_sc_channel<6, sc_dt::sc_bv<6> >();
        sc_inst_1->axi_awaddr.bind(*p_1_axi_awaddr);
        (*_glb_hdl_sc_channel_map)[39458].first = 3;
        (*_glb_hdl_sc_channel_map)[39458].second.push_front(p_1_axi_awaddr);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_1_axi_awprot = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_1->axi_awprot.bind(*p_1_axi_awprot);
        (*_glb_hdl_sc_channel_map)[39459].first = 3;
        (*_glb_hdl_sc_channel_map)[39459].second.push_front(p_1_axi_awprot);
        xsim_sc_channel<0, bool > *p_1_axi_awvalid = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_awvalid.bind(*p_1_axi_awvalid);
        (*_glb_hdl_sc_channel_map)[39461].first = 1;
        (*_glb_hdl_sc_channel_map)[39461].second.push_front(p_1_axi_awvalid);
        xsim_sc_channel<0, bool > *p_1_axi_awready = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_awready.bind(*p_1_axi_awready);
        (*_glb_hdl_sc_channel_map)[39460].first = 1;
        (*_glb_hdl_sc_channel_map)[39460].second.push_front(p_1_axi_awready);
        xsim_sc_channel<32, sc_dt::sc_bv<32> > *p_1_axi_wdata = new xsim_sc_channel<32, sc_dt::sc_bv<32> >();
        sc_inst_1->axi_wdata.bind(*p_1_axi_wdata);
        (*_glb_hdl_sc_channel_map)[39469].first = 3;
        (*_glb_hdl_sc_channel_map)[39469].second.push_front(p_1_axi_wdata);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_1_axi_wstrb = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_1->axi_wstrb.bind(*p_1_axi_wstrb);
        (*_glb_hdl_sc_channel_map)[39471].first = 3;
        (*_glb_hdl_sc_channel_map)[39471].second.push_front(p_1_axi_wstrb);
        xsim_sc_channel<0, bool > *p_1_axi_wvalid = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_wvalid.bind(*p_1_axi_wvalid);
        (*_glb_hdl_sc_channel_map)[39472].first = 1;
        (*_glb_hdl_sc_channel_map)[39472].second.push_front(p_1_axi_wvalid);
        xsim_sc_channel<0, bool > *p_1_axi_wready = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_wready.bind(*p_1_axi_wready);
        (*_glb_hdl_sc_channel_map)[39470].first = 1;
        (*_glb_hdl_sc_channel_map)[39470].second.push_front(p_1_axi_wready);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_1_axi_bresp = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_1->axi_bresp.bind(*p_1_axi_bresp);
        (*_glb_hdl_sc_channel_map)[39463].first = 3;
        (*_glb_hdl_sc_channel_map)[39463].second.push_front(p_1_axi_bresp);
        xsim_sc_channel<0, bool > *p_1_axi_bvalid = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_bvalid.bind(*p_1_axi_bvalid);
        (*_glb_hdl_sc_channel_map)[39464].first = 1;
        (*_glb_hdl_sc_channel_map)[39464].second.push_front(p_1_axi_bvalid);
        xsim_sc_channel<0, bool > *p_1_axi_bready = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_bready.bind(*p_1_axi_bready);
        (*_glb_hdl_sc_channel_map)[39462].first = 1;
        (*_glb_hdl_sc_channel_map)[39462].second.push_front(p_1_axi_bready);
        xsim_sc_channel<6, sc_dt::sc_bv<6> > *p_1_axi_araddr = new xsim_sc_channel<6, sc_dt::sc_bv<6> >();
        sc_inst_1->axi_araddr.bind(*p_1_axi_araddr);
        (*_glb_hdl_sc_channel_map)[39453].first = 3;
        (*_glb_hdl_sc_channel_map)[39453].second.push_front(p_1_axi_araddr);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_1_axi_arprot = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_1->axi_arprot.bind(*p_1_axi_arprot);
        (*_glb_hdl_sc_channel_map)[39455].first = 3;
        (*_glb_hdl_sc_channel_map)[39455].second.push_front(p_1_axi_arprot);
        xsim_sc_channel<0, bool > *p_1_axi_arvalid = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_arvalid.bind(*p_1_axi_arvalid);
        (*_glb_hdl_sc_channel_map)[39457].first = 1;
        (*_glb_hdl_sc_channel_map)[39457].second.push_front(p_1_axi_arvalid);
        xsim_sc_channel<0, bool > *p_1_axi_arready = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_arready.bind(*p_1_axi_arready);
        (*_glb_hdl_sc_channel_map)[39456].first = 1;
        (*_glb_hdl_sc_channel_map)[39456].second.push_front(p_1_axi_arready);
        xsim_sc_channel<32, sc_dt::sc_bv<32> > *p_1_axi_rdata = new xsim_sc_channel<32, sc_dt::sc_bv<32> >();
        sc_inst_1->axi_rdata.bind(*p_1_axi_rdata);
        (*_glb_hdl_sc_channel_map)[39465].first = 3;
        (*_glb_hdl_sc_channel_map)[39465].second.push_front(p_1_axi_rdata);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_1_axi_rresp = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_1->axi_rresp.bind(*p_1_axi_rresp);
        (*_glb_hdl_sc_channel_map)[39467].first = 3;
        (*_glb_hdl_sc_channel_map)[39467].second.push_front(p_1_axi_rresp);
        xsim_sc_channel<0, bool > *p_1_axi_rvalid = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_rvalid.bind(*p_1_axi_rvalid);
        (*_glb_hdl_sc_channel_map)[39468].first = 1;
        (*_glb_hdl_sc_channel_map)[39468].second.push_front(p_1_axi_rvalid);
        xsim_sc_channel<0, bool > *p_1_axi_rready = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_rready.bind(*p_1_axi_rready);
        (*_glb_hdl_sc_channel_map)[39466].first = 1;
        (*_glb_hdl_sc_channel_map)[39466].second.push_front(p_1_axi_rready);
        xsim_sc_channel<0, bool > *p_1_axi_aclk = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_aclk.bind(*p_1_axi_aclk);
        (*_glb_hdl_sc_channel_map)[39452].first = 1;
        (*_glb_hdl_sc_channel_map)[39452].second.push_front(p_1_axi_aclk);
        xsim_sc_channel<0, bool > *p_1_axi_aresetn = new xsim_sc_channel<0, bool >();
        sc_inst_1->axi_aresetn.bind(*p_1_axi_aresetn);
        (*_glb_hdl_sc_channel_map)[39454].first = 1;
        (*_glb_hdl_sc_channel_map)[39454].second.push_front(p_1_axi_aresetn);
        emu_sim_axi_perf_mon2_0_0 *sc_inst_2 = new emu_sim_axi_perf_mon2_0_0(".emu_wrapper.emu_i.sim_axi_perf_mon2_0");
        xsim_sc_channel<64, sc_dt::sc_bv<64> > *p_2_axi_awaddr = new xsim_sc_channel<64, sc_dt::sc_bv<64> >();
        sc_inst_2->axi_awaddr.bind(*p_2_axi_awaddr);
        (*_glb_hdl_sc_channel_map)[39486].first = 3;
        (*_glb_hdl_sc_channel_map)[39486].second.push_front(p_2_axi_awaddr);
        xsim_sc_channel<8, sc_dt::sc_bv<8> > *p_2_axi_awlen = new xsim_sc_channel<8, sc_dt::sc_bv<8> >();
        sc_inst_2->axi_awlen.bind(*p_2_axi_awlen);
        (*_glb_hdl_sc_channel_map)[39489].first = 3;
        (*_glb_hdl_sc_channel_map)[39489].second.push_front(p_2_axi_awlen);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_2_axi_awsize = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_2->axi_awsize.bind(*p_2_axi_awsize);
        (*_glb_hdl_sc_channel_map)[39495].first = 3;
        (*_glb_hdl_sc_channel_map)[39495].second.push_front(p_2_axi_awsize);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_2_axi_awburst = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_2->axi_awburst.bind(*p_2_axi_awburst);
        (*_glb_hdl_sc_channel_map)[39487].first = 3;
        (*_glb_hdl_sc_channel_map)[39487].second.push_front(p_2_axi_awburst);
        xsim_sc_channel<0, bool > *p_2_axi_awlock = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_awlock.bind(*p_2_axi_awlock);
        (*_glb_hdl_sc_channel_map)[39490].first = 1;
        (*_glb_hdl_sc_channel_map)[39490].second.push_front(p_2_axi_awlock);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_2_axi_awcache = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_2->axi_awcache.bind(*p_2_axi_awcache);
        (*_glb_hdl_sc_channel_map)[39488].first = 3;
        (*_glb_hdl_sc_channel_map)[39488].second.push_front(p_2_axi_awcache);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_2_axi_awprot = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_2->axi_awprot.bind(*p_2_axi_awprot);
        (*_glb_hdl_sc_channel_map)[39491].first = 3;
        (*_glb_hdl_sc_channel_map)[39491].second.push_front(p_2_axi_awprot);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_2_axi_awregion = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_2->axi_awregion.bind(*p_2_axi_awregion);
        (*_glb_hdl_sc_channel_map)[39494].first = 3;
        (*_glb_hdl_sc_channel_map)[39494].second.push_front(p_2_axi_awregion);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_2_axi_awqos = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_2->axi_awqos.bind(*p_2_axi_awqos);
        (*_glb_hdl_sc_channel_map)[39492].first = 3;
        (*_glb_hdl_sc_channel_map)[39492].second.push_front(p_2_axi_awqos);
        xsim_sc_channel<0, bool > *p_2_axi_awvalid = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_awvalid.bind(*p_2_axi_awvalid);
        (*_glb_hdl_sc_channel_map)[39496].first = 1;
        (*_glb_hdl_sc_channel_map)[39496].second.push_front(p_2_axi_awvalid);
        xsim_sc_channel<0, bool > *p_2_axi_awready = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_awready.bind(*p_2_axi_awready);
        (*_glb_hdl_sc_channel_map)[39493].first = 1;
        (*_glb_hdl_sc_channel_map)[39493].second.push_front(p_2_axi_awready);
        xsim_sc_channel<64, sc_dt::sc_bv<64> > *p_2_axi_wdata = new xsim_sc_channel<64, sc_dt::sc_bv<64> >();
        sc_inst_2->axi_wdata.bind(*p_2_axi_wdata);
        (*_glb_hdl_sc_channel_map)[39505].first = 3;
        (*_glb_hdl_sc_channel_map)[39505].second.push_front(p_2_axi_wdata);
        xsim_sc_channel<8, sc_dt::sc_bv<8> > *p_2_axi_wstrb = new xsim_sc_channel<8, sc_dt::sc_bv<8> >();
        sc_inst_2->axi_wstrb.bind(*p_2_axi_wstrb);
        (*_glb_hdl_sc_channel_map)[39508].first = 3;
        (*_glb_hdl_sc_channel_map)[39508].second.push_front(p_2_axi_wstrb);
        xsim_sc_channel<0, bool > *p_2_axi_wlast = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_wlast.bind(*p_2_axi_wlast);
        (*_glb_hdl_sc_channel_map)[39506].first = 1;
        (*_glb_hdl_sc_channel_map)[39506].second.push_front(p_2_axi_wlast);
        xsim_sc_channel<0, bool > *p_2_axi_wvalid = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_wvalid.bind(*p_2_axi_wvalid);
        (*_glb_hdl_sc_channel_map)[39509].first = 1;
        (*_glb_hdl_sc_channel_map)[39509].second.push_front(p_2_axi_wvalid);
        xsim_sc_channel<0, bool > *p_2_axi_wready = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_wready.bind(*p_2_axi_wready);
        (*_glb_hdl_sc_channel_map)[39507].first = 1;
        (*_glb_hdl_sc_channel_map)[39507].second.push_front(p_2_axi_wready);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_2_axi_bresp = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_2->axi_bresp.bind(*p_2_axi_bresp);
        (*_glb_hdl_sc_channel_map)[39498].first = 3;
        (*_glb_hdl_sc_channel_map)[39498].second.push_front(p_2_axi_bresp);
        xsim_sc_channel<0, bool > *p_2_axi_bvalid = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_bvalid.bind(*p_2_axi_bvalid);
        (*_glb_hdl_sc_channel_map)[39499].first = 1;
        (*_glb_hdl_sc_channel_map)[39499].second.push_front(p_2_axi_bvalid);
        xsim_sc_channel<0, bool > *p_2_axi_bready = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_bready.bind(*p_2_axi_bready);
        (*_glb_hdl_sc_channel_map)[39497].first = 1;
        (*_glb_hdl_sc_channel_map)[39497].second.push_front(p_2_axi_bready);
        xsim_sc_channel<64, sc_dt::sc_bv<64> > *p_2_axi_araddr = new xsim_sc_channel<64, sc_dt::sc_bv<64> >();
        sc_inst_2->axi_araddr.bind(*p_2_axi_araddr);
        (*_glb_hdl_sc_channel_map)[39474].first = 3;
        (*_glb_hdl_sc_channel_map)[39474].second.push_front(p_2_axi_araddr);
        xsim_sc_channel<8, sc_dt::sc_bv<8> > *p_2_axi_arlen = new xsim_sc_channel<8, sc_dt::sc_bv<8> >();
        sc_inst_2->axi_arlen.bind(*p_2_axi_arlen);
        (*_glb_hdl_sc_channel_map)[39478].first = 3;
        (*_glb_hdl_sc_channel_map)[39478].second.push_front(p_2_axi_arlen);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_2_axi_arsize = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_2->axi_arsize.bind(*p_2_axi_arsize);
        (*_glb_hdl_sc_channel_map)[39484].first = 3;
        (*_glb_hdl_sc_channel_map)[39484].second.push_front(p_2_axi_arsize);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_2_axi_arburst = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_2->axi_arburst.bind(*p_2_axi_arburst);
        (*_glb_hdl_sc_channel_map)[39475].first = 3;
        (*_glb_hdl_sc_channel_map)[39475].second.push_front(p_2_axi_arburst);
        xsim_sc_channel<0, bool > *p_2_axi_arlock = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_arlock.bind(*p_2_axi_arlock);
        (*_glb_hdl_sc_channel_map)[39479].first = 1;
        (*_glb_hdl_sc_channel_map)[39479].second.push_front(p_2_axi_arlock);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_2_axi_arcache = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_2->axi_arcache.bind(*p_2_axi_arcache);
        (*_glb_hdl_sc_channel_map)[39476].first = 3;
        (*_glb_hdl_sc_channel_map)[39476].second.push_front(p_2_axi_arcache);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_2_axi_arprot = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_2->axi_arprot.bind(*p_2_axi_arprot);
        (*_glb_hdl_sc_channel_map)[39480].first = 3;
        (*_glb_hdl_sc_channel_map)[39480].second.push_front(p_2_axi_arprot);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_2_axi_arregion = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_2->axi_arregion.bind(*p_2_axi_arregion);
        (*_glb_hdl_sc_channel_map)[39483].first = 3;
        (*_glb_hdl_sc_channel_map)[39483].second.push_front(p_2_axi_arregion);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_2_axi_arqos = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_2->axi_arqos.bind(*p_2_axi_arqos);
        (*_glb_hdl_sc_channel_map)[39481].first = 3;
        (*_glb_hdl_sc_channel_map)[39481].second.push_front(p_2_axi_arqos);
        xsim_sc_channel<0, bool > *p_2_axi_arvalid = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_arvalid.bind(*p_2_axi_arvalid);
        (*_glb_hdl_sc_channel_map)[39485].first = 1;
        (*_glb_hdl_sc_channel_map)[39485].second.push_front(p_2_axi_arvalid);
        xsim_sc_channel<0, bool > *p_2_axi_arready = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_arready.bind(*p_2_axi_arready);
        (*_glb_hdl_sc_channel_map)[39482].first = 1;
        (*_glb_hdl_sc_channel_map)[39482].second.push_front(p_2_axi_arready);
        xsim_sc_channel<64, sc_dt::sc_bv<64> > *p_2_axi_rdata = new xsim_sc_channel<64, sc_dt::sc_bv<64> >();
        sc_inst_2->axi_rdata.bind(*p_2_axi_rdata);
        (*_glb_hdl_sc_channel_map)[39500].first = 3;
        (*_glb_hdl_sc_channel_map)[39500].second.push_front(p_2_axi_rdata);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_2_axi_rresp = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_2->axi_rresp.bind(*p_2_axi_rresp);
        (*_glb_hdl_sc_channel_map)[39503].first = 3;
        (*_glb_hdl_sc_channel_map)[39503].second.push_front(p_2_axi_rresp);
        xsim_sc_channel<0, bool > *p_2_axi_rlast = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_rlast.bind(*p_2_axi_rlast);
        (*_glb_hdl_sc_channel_map)[39501].first = 1;
        (*_glb_hdl_sc_channel_map)[39501].second.push_front(p_2_axi_rlast);
        xsim_sc_channel<0, bool > *p_2_axi_rvalid = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_rvalid.bind(*p_2_axi_rvalid);
        (*_glb_hdl_sc_channel_map)[39504].first = 1;
        (*_glb_hdl_sc_channel_map)[39504].second.push_front(p_2_axi_rvalid);
        xsim_sc_channel<0, bool > *p_2_axi_rready = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_rready.bind(*p_2_axi_rready);
        (*_glb_hdl_sc_channel_map)[39502].first = 1;
        (*_glb_hdl_sc_channel_map)[39502].second.push_front(p_2_axi_rready);
        xsim_sc_channel<0, bool > *p_2_axi_aclk = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_aclk.bind(*p_2_axi_aclk);
        (*_glb_hdl_sc_channel_map)[39473].first = 1;
        (*_glb_hdl_sc_channel_map)[39473].second.push_front(p_2_axi_aclk);
        xsim_sc_channel<0, bool > *p_2_axi_aresetn = new xsim_sc_channel<0, bool >();
        sc_inst_2->axi_aresetn.bind(*p_2_axi_aresetn);
        (*_glb_hdl_sc_channel_map)[39477].first = 1;
        (*_glb_hdl_sc_channel_map)[39477].second.push_front(p_2_axi_aresetn);
        emu_sim_axi_perf_mon2_1_0 *sc_inst_3 = new emu_sim_axi_perf_mon2_1_0(".emu_wrapper.emu_i.sim_axi_perf_mon2_1");
        xsim_sc_channel<64, sc_dt::sc_bv<64> > *p_3_axi_awaddr = new xsim_sc_channel<64, sc_dt::sc_bv<64> >();
        sc_inst_3->axi_awaddr.bind(*p_3_axi_awaddr);
        (*_glb_hdl_sc_channel_map)[39523].first = 3;
        (*_glb_hdl_sc_channel_map)[39523].second.push_front(p_3_axi_awaddr);
        xsim_sc_channel<8, sc_dt::sc_bv<8> > *p_3_axi_awlen = new xsim_sc_channel<8, sc_dt::sc_bv<8> >();
        sc_inst_3->axi_awlen.bind(*p_3_axi_awlen);
        (*_glb_hdl_sc_channel_map)[39526].first = 3;
        (*_glb_hdl_sc_channel_map)[39526].second.push_front(p_3_axi_awlen);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_3_axi_awsize = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_3->axi_awsize.bind(*p_3_axi_awsize);
        (*_glb_hdl_sc_channel_map)[39532].first = 3;
        (*_glb_hdl_sc_channel_map)[39532].second.push_front(p_3_axi_awsize);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_3_axi_awburst = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_3->axi_awburst.bind(*p_3_axi_awburst);
        (*_glb_hdl_sc_channel_map)[39524].first = 3;
        (*_glb_hdl_sc_channel_map)[39524].second.push_front(p_3_axi_awburst);
        xsim_sc_channel<0, bool > *p_3_axi_awlock = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_awlock.bind(*p_3_axi_awlock);
        (*_glb_hdl_sc_channel_map)[39527].first = 1;
        (*_glb_hdl_sc_channel_map)[39527].second.push_front(p_3_axi_awlock);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_3_axi_awcache = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_3->axi_awcache.bind(*p_3_axi_awcache);
        (*_glb_hdl_sc_channel_map)[39525].first = 3;
        (*_glb_hdl_sc_channel_map)[39525].second.push_front(p_3_axi_awcache);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_3_axi_awprot = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_3->axi_awprot.bind(*p_3_axi_awprot);
        (*_glb_hdl_sc_channel_map)[39528].first = 3;
        (*_glb_hdl_sc_channel_map)[39528].second.push_front(p_3_axi_awprot);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_3_axi_awregion = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_3->axi_awregion.bind(*p_3_axi_awregion);
        (*_glb_hdl_sc_channel_map)[39531].first = 3;
        (*_glb_hdl_sc_channel_map)[39531].second.push_front(p_3_axi_awregion);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_3_axi_awqos = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_3->axi_awqos.bind(*p_3_axi_awqos);
        (*_glb_hdl_sc_channel_map)[39529].first = 3;
        (*_glb_hdl_sc_channel_map)[39529].second.push_front(p_3_axi_awqos);
        xsim_sc_channel<0, bool > *p_3_axi_awvalid = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_awvalid.bind(*p_3_axi_awvalid);
        (*_glb_hdl_sc_channel_map)[39533].first = 1;
        (*_glb_hdl_sc_channel_map)[39533].second.push_front(p_3_axi_awvalid);
        xsim_sc_channel<0, bool > *p_3_axi_awready = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_awready.bind(*p_3_axi_awready);
        (*_glb_hdl_sc_channel_map)[39530].first = 1;
        (*_glb_hdl_sc_channel_map)[39530].second.push_front(p_3_axi_awready);
        xsim_sc_channel<32, sc_dt::sc_bv<32> > *p_3_axi_wdata = new xsim_sc_channel<32, sc_dt::sc_bv<32> >();
        sc_inst_3->axi_wdata.bind(*p_3_axi_wdata);
        (*_glb_hdl_sc_channel_map)[39542].first = 3;
        (*_glb_hdl_sc_channel_map)[39542].second.push_front(p_3_axi_wdata);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_3_axi_wstrb = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_3->axi_wstrb.bind(*p_3_axi_wstrb);
        (*_glb_hdl_sc_channel_map)[39545].first = 3;
        (*_glb_hdl_sc_channel_map)[39545].second.push_front(p_3_axi_wstrb);
        xsim_sc_channel<0, bool > *p_3_axi_wlast = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_wlast.bind(*p_3_axi_wlast);
        (*_glb_hdl_sc_channel_map)[39543].first = 1;
        (*_glb_hdl_sc_channel_map)[39543].second.push_front(p_3_axi_wlast);
        xsim_sc_channel<0, bool > *p_3_axi_wvalid = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_wvalid.bind(*p_3_axi_wvalid);
        (*_glb_hdl_sc_channel_map)[39546].first = 1;
        (*_glb_hdl_sc_channel_map)[39546].second.push_front(p_3_axi_wvalid);
        xsim_sc_channel<0, bool > *p_3_axi_wready = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_wready.bind(*p_3_axi_wready);
        (*_glb_hdl_sc_channel_map)[39544].first = 1;
        (*_glb_hdl_sc_channel_map)[39544].second.push_front(p_3_axi_wready);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_3_axi_bresp = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_3->axi_bresp.bind(*p_3_axi_bresp);
        (*_glb_hdl_sc_channel_map)[39535].first = 3;
        (*_glb_hdl_sc_channel_map)[39535].second.push_front(p_3_axi_bresp);
        xsim_sc_channel<0, bool > *p_3_axi_bvalid = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_bvalid.bind(*p_3_axi_bvalid);
        (*_glb_hdl_sc_channel_map)[39536].first = 1;
        (*_glb_hdl_sc_channel_map)[39536].second.push_front(p_3_axi_bvalid);
        xsim_sc_channel<0, bool > *p_3_axi_bready = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_bready.bind(*p_3_axi_bready);
        (*_glb_hdl_sc_channel_map)[39534].first = 1;
        (*_glb_hdl_sc_channel_map)[39534].second.push_front(p_3_axi_bready);
        xsim_sc_channel<64, sc_dt::sc_bv<64> > *p_3_axi_araddr = new xsim_sc_channel<64, sc_dt::sc_bv<64> >();
        sc_inst_3->axi_araddr.bind(*p_3_axi_araddr);
        (*_glb_hdl_sc_channel_map)[39511].first = 3;
        (*_glb_hdl_sc_channel_map)[39511].second.push_front(p_3_axi_araddr);
        xsim_sc_channel<8, sc_dt::sc_bv<8> > *p_3_axi_arlen = new xsim_sc_channel<8, sc_dt::sc_bv<8> >();
        sc_inst_3->axi_arlen.bind(*p_3_axi_arlen);
        (*_glb_hdl_sc_channel_map)[39515].first = 3;
        (*_glb_hdl_sc_channel_map)[39515].second.push_front(p_3_axi_arlen);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_3_axi_arsize = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_3->axi_arsize.bind(*p_3_axi_arsize);
        (*_glb_hdl_sc_channel_map)[39521].first = 3;
        (*_glb_hdl_sc_channel_map)[39521].second.push_front(p_3_axi_arsize);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_3_axi_arburst = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_3->axi_arburst.bind(*p_3_axi_arburst);
        (*_glb_hdl_sc_channel_map)[39512].first = 3;
        (*_glb_hdl_sc_channel_map)[39512].second.push_front(p_3_axi_arburst);
        xsim_sc_channel<0, bool > *p_3_axi_arlock = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_arlock.bind(*p_3_axi_arlock);
        (*_glb_hdl_sc_channel_map)[39516].first = 1;
        (*_glb_hdl_sc_channel_map)[39516].second.push_front(p_3_axi_arlock);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_3_axi_arcache = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_3->axi_arcache.bind(*p_3_axi_arcache);
        (*_glb_hdl_sc_channel_map)[39513].first = 3;
        (*_glb_hdl_sc_channel_map)[39513].second.push_front(p_3_axi_arcache);
        xsim_sc_channel<3, sc_dt::sc_bv<3> > *p_3_axi_arprot = new xsim_sc_channel<3, sc_dt::sc_bv<3> >();
        sc_inst_3->axi_arprot.bind(*p_3_axi_arprot);
        (*_glb_hdl_sc_channel_map)[39517].first = 3;
        (*_glb_hdl_sc_channel_map)[39517].second.push_front(p_3_axi_arprot);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_3_axi_arregion = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_3->axi_arregion.bind(*p_3_axi_arregion);
        (*_glb_hdl_sc_channel_map)[39520].first = 3;
        (*_glb_hdl_sc_channel_map)[39520].second.push_front(p_3_axi_arregion);
        xsim_sc_channel<4, sc_dt::sc_bv<4> > *p_3_axi_arqos = new xsim_sc_channel<4, sc_dt::sc_bv<4> >();
        sc_inst_3->axi_arqos.bind(*p_3_axi_arqos);
        (*_glb_hdl_sc_channel_map)[39518].first = 3;
        (*_glb_hdl_sc_channel_map)[39518].second.push_front(p_3_axi_arqos);
        xsim_sc_channel<0, bool > *p_3_axi_arvalid = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_arvalid.bind(*p_3_axi_arvalid);
        (*_glb_hdl_sc_channel_map)[39522].first = 1;
        (*_glb_hdl_sc_channel_map)[39522].second.push_front(p_3_axi_arvalid);
        xsim_sc_channel<0, bool > *p_3_axi_arready = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_arready.bind(*p_3_axi_arready);
        (*_glb_hdl_sc_channel_map)[39519].first = 1;
        (*_glb_hdl_sc_channel_map)[39519].second.push_front(p_3_axi_arready);
        xsim_sc_channel<32, sc_dt::sc_bv<32> > *p_3_axi_rdata = new xsim_sc_channel<32, sc_dt::sc_bv<32> >();
        sc_inst_3->axi_rdata.bind(*p_3_axi_rdata);
        (*_glb_hdl_sc_channel_map)[39537].first = 3;
        (*_glb_hdl_sc_channel_map)[39537].second.push_front(p_3_axi_rdata);
        xsim_sc_channel<2, sc_dt::sc_bv<2> > *p_3_axi_rresp = new xsim_sc_channel<2, sc_dt::sc_bv<2> >();
        sc_inst_3->axi_rresp.bind(*p_3_axi_rresp);
        (*_glb_hdl_sc_channel_map)[39540].first = 3;
        (*_glb_hdl_sc_channel_map)[39540].second.push_front(p_3_axi_rresp);
        xsim_sc_channel<0, bool > *p_3_axi_rlast = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_rlast.bind(*p_3_axi_rlast);
        (*_glb_hdl_sc_channel_map)[39538].first = 1;
        (*_glb_hdl_sc_channel_map)[39538].second.push_front(p_3_axi_rlast);
        xsim_sc_channel<0, bool > *p_3_axi_rvalid = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_rvalid.bind(*p_3_axi_rvalid);
        (*_glb_hdl_sc_channel_map)[39541].first = 1;
        (*_glb_hdl_sc_channel_map)[39541].second.push_front(p_3_axi_rvalid);
        xsim_sc_channel<0, bool > *p_3_axi_rready = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_rready.bind(*p_3_axi_rready);
        (*_glb_hdl_sc_channel_map)[39539].first = 1;
        (*_glb_hdl_sc_channel_map)[39539].second.push_front(p_3_axi_rready);
        xsim_sc_channel<0, bool > *p_3_axi_aclk = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_aclk.bind(*p_3_axi_aclk);
        (*_glb_hdl_sc_channel_map)[39510].first = 1;
        (*_glb_hdl_sc_channel_map)[39510].second.push_front(p_3_axi_aclk);
        xsim_sc_channel<0, bool > *p_3_axi_aresetn = new xsim_sc_channel<0, bool >();
        sc_inst_3->axi_aresetn.bind(*p_3_axi_aresetn);
        (*_glb_hdl_sc_channel_map)[39514].first = 1;
        (*_glb_hdl_sc_channel_map)[39514].second.push_front(p_3_axi_aresetn);
    }
