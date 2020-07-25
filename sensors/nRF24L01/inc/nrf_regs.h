#ifndef NRF_REGS_H_
#define NRF_REGS_H_

#define NRF_CONFIG_ADDR                   0x00
#define NRF_CONFIG_MASK_RX_DR_SHFT        0x06
#define NRF_CONFIG_MASK_RX_DR_BMSK        0x40
#define NRF_CONFIG_MASK_TX_DS_SHFT        0x05
#define NRF_CONFIG_MASK_TX_DS_BMSK        0x20
#define NRF_CONFIG_MASK_MAX_RT_SHFT       0x04
#define NRF_CONFIG_MASK_MAX_RT_BMSK       0x10
#define NRF_CONFIG_EN_CRC_SHFT            0x03
#define NRF_CONFIG_EN_CRC_BMSK            0x08
#define NRF_CONFIG_CRCO_SHFT              0x02
#define NRF_CONFIG_CRCO_BMSK              0x04
#define NRF_CONFIG_PWR_UP_SHFT            0x01
#define NRF_CONFIG_PWR_UP_BMSK            0x02
#define NRF_CONFIG_PRIM_RX_SHFT           0x00
#define NRF_CONFIG_PRIM_RX_BMSK           0x01

#define NRF_EN_AA_ADDR                    0x01
#define NRF_EN_AA_ENAA_P5_SHFT            0x05
#define NRF_EN_AA_ENAA_P5_BMSK            0x20
#define NRF_EN_AA_ENAA_P4_SHFT            0x04
#define NRF_EN_AA_ENAA_P4_BMSK            0x10
#define NRF_EN_AA_ENAA_P3_SHFT            0x03
#define NRF_EN_AA_ENAA_P3_BMSK            0x08
#define NRF_EN_AA_ENAA_P2_SHFT            0x02
#define NRF_EN_AA_ENAA_P2_BMSK            0x04
#define NRF_EN_AA_ENAA_P1_SHFT            0x01
#define NRF_EN_AA_ENAA_P1_BMSK            0x02
#define NRF_EN_AA_ENAA_P0_SHFT            0x00
#define NRF_EN_AA_ENAA_P0_BMSK            0x01

#define NRF_EN_RXADDR_ADDR                0x02
#define NRF_EN_RXADDR_ERX_P5_SHFT         0x05
#define NRF_EN_RXADDR_ERX_P5_BMSK         0x20
#define NRF_EN_RXADDR_ERX_P4_SHFT         0x04
#define NRF_EN_RXADDR_ERX_P4_BMSK         0x10
#define NRF_EN_RXADDR_ERX_P3_SHFT         0x03
#define NRF_EN_RXADDR_ERX_P3_BMSK         0x08
#define NRF_EN_RXADDR_ERX_P2_SHFT         0x02
#define NRF_EN_RXADDR_ERX_P2_BMSK         0x04
#define NRF_EN_RXADDR_ERX_P1_SHFT         0x01
#define NRF_EN_RXADDR_ERX_P1_BMSK         0x02
#define NRF_EN_RXADDR_ERX_P0_SHFT         0x00
#define NRF_EN_RXADDR_ERX_P0_BMSK         0x01

#define NRF_SETUP_AW_ADDR                 0x03
#define NRF_SETUP_AW_AW_SHFT              0x00
#define NRF_SETUP_AW_AW_BMSK              0x03
#define NRF_SETUP_RETR_ADDR               0x04
#define NRF_SETUP_RETR_ARD_SHFT           0x04
#define NRF_SETUP_RETR_ARD_BMSK           0xf0
#define NRF_SETUP_RETR_ARC_SHFT           0x00
#define NRF_SETUP_RETR_ARC_BMSK           0x0f

#define NRF_RF_CH_ADDR                    0x05
#define NRF_RF_CH_RF_CH_SHFT              0x00
#define NRF_RF_CH_RF_CH_BMSK              0x7f

#define NRF_RF_SETUP_ADDR                 0x06
#define NRF_RF_SETUP_CONT_WAVE_SHFT       0x07
#define NRF_RF_SETUP_CONT_WAVE_BMSK       0x80
#define NRF_RF_SETUP_RF_DR_LOW_SHFT       0x05
#define NRF_RF_SETUP_RF_DR_LOW_BMSK       0x20
#define NRF_RF_SETUP_PLL_LOCK_SHFT        0x04
#define NRF_RF_SETUP_PLL_LOCK_BMSK        0x10
#define NRF_RF_SETUP_RF_DR_HIGH_SHFT      0x03
#define NRF_RF_SETUP_RF_DR_HIGH_BMSK      0x08
#define NRF_RF_SETUP_RF_PWR_SHFT          0x01
#define NRF_RF_SETUP_RF_PWR_BMSK          0x06

#define NRF_STATUS_ADDR                   0x07
#define NRF_STATUS_RX_DR_SHFT             0x06
#define NRF_STATUS_RX_DR_BMSK             0x40
#define NRF_STATUS_TX_DS_SHFT             0x05
#define NRF_STATUS_TX_DS_BMSK             0x20
#define NRF_STATUS_MAX_RT_SHFT            0x04
#define NRF_STATUS_MAX_RT_BMSK            0x10
#define NRF_STATUS_RX_P_NO_SHFT           0x01
#define NRF_STATUS_RX_P_NO_BMSK           0x0e
#define NRF_STATUS_TX_FULL_SHFT           0x00
#define NRF_STATUS_TX_FULL_BMSK           0x01

#define NRF_OBSERVE_TX_ADDR               0x08
#define NRF_OBSERVE_TX_PLOS_CNT_SHFT      0x04
#define NRF_OBSERVE_TX_PLOS_CNT_BMSK      0xf0
#define NRF_OBSERVE_TX_ARC_CNT_SHFT       0x00
#define NRF_OBSERVE_TX_ARC_CNT_BMSK       0x0f

#define NRF_RPD_ADDR                      0x09
#define NRF_RPD_RPD_SHFT                  0x00
#define NRF_RPD_RPD_BMSK                  0x01

#define NRF_RX_ADDR_P0_ADDR               0x0A

#define NRF_RX_ADDR_P1_ADDR               0x0B

#define NRF_RX_ADDR_P2_ADDR               0x0C

#define NRF_RX_ADDR_P3_ADDR               0x0D

#define NRF_RX_ADDR_P4_ADDR               0x0E

#define NRF_RX_ADDR_P5_ADDR               0x0F

#define NRF_TX_ADDR_ADDR                  0x10

#define NRF_RX_PW_P0_ADDR                 0x11
#define NRF_RX_PW_P0_RX_PW_P0_SHFT        0x00
#define NRF_RX_PW_P0_RX_PW_P0_BMSK        0x3f

#define NRF_RX_PW_P1_ADDR                 0x12
#define NRF_RX_PW_P1_RX_PW_P1_SHFT        0x00
#define NRF_RX_PW_P1_RX_PW_P1_BMSK        0x3f

#define NRF_RX_PW_P2_ADDR                 0x13
#define NRF_RX_PW_P2_RX_PW_P2_SHFT        0x00
#define NRF_RX_PW_P2_RX_PW_P2_BMSK        0x3f

#define NRF_RX_PW_P3_ADDR                 0x14
#define NRF_RX_PW_P3_RX_PW_P3_SHFT        0x00
#define NRF_RX_PW_P3_RX_PW_P3_BMSK        0x3f

#define NRF_RX_PW_P4_ADDR                 0x15
#define NRF_RX_PW_P4_RX_PW_P4_SHFT        0x00
#define NRF_RX_PW_P4_RX_PW_P4_BMSK        0x3f

#define NRF_RX_PW_P5_ADDR                 0x16
#define NRF_RX_PW_P5_RX_PW_P5_SHFT        0x00
#define NRF_RX_PW_P5_RX_PW_P5_BMSK        0x3f

#define NRF_FIFO_STATUS_ADDR              0x17
#define NRF_FIFO_STATUS_TX_REUSE_SHFT     0x06
#define NRF_FIFO_STATUS_TX_REUSE_BMSK     0x40
#define NRF_FIFO_STATUS_TX_FULL_SHFT      0x05
#define NRF_FIFO_STATUS_TX_FULL_BMSK      0x20
#define NRF_FIFO_STATUS_TX_EMPTY_SHFT     0x04
#define NRF_FIFO_STATUS_TX_EMPTY_BMSK     0x10
#define NRF_FIFO_STATUS_RX_FULL_SHFT      0x01
#define NRF_FIFO_STATUS_RX_FULL_BMSK      0x02
#define NRF_FIFO_STATUS_RX_EMPTY_SHFT     0x00
#define NRF_FIFO_STATUS_RX_EMPTY_BMSK     0x01

#define NRF_DYNPD_ADDR                    0x1C
#define NRF_DYNPD_DPL_P5_SHFT             0x05
#define NRF_DYNPD_DPL_P5_BMSK             0x20
#define NRF_DYNPD_DPL_P4_SHFT             0x04
#define NRF_DYNPD_DPL_P4_BMSK             0x10
#define NRF_DYNPD_DPL_P3_SHFT             0x03
#define NRF_DYNPD_DPL_P3_BMSK             0x08
#define NRF_DYNPD_DPL_P2_SHFT             0x02
#define NRF_DYNPD_DPL_P2_BMSK             0x04
#define NRF_DYNPD_DPL_P1_SHFT             0x01
#define NRF_DYNPD_DPL_P1_BMSK             0x02
#define NRF_DYNPD_DPL_P0_SHFT             0x00
#define NRF_DYNPD_DPL_P0_BMSK             0x01

#define NRF_FEATURE_ADDR                  0x1D
#define NRF_FEATURE_EN_DPL_SHFT           0x02
#define NRF_FEATURE_EN_DPL_BMSK           0x04
#define NRF_FEATURE_EN_ACK_PAY_SHFT       0x01
#define NRF_FEATURE_EN_ACK_PAY_BMSK       0x02
#define NRF_FEATURE_EN_DYN_ACK_SHFT       0x00
#define NRF_FEATURE_EN_DYN_ACK_BMSK       0x01

#endif
