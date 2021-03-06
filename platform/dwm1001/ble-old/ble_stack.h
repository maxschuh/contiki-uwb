/**
 * \name Network Stack Configuration
 *
 * @{
 */
#ifndef NETSTACK_CONF_NETWORK
#define NETSTACK_CONF_NETWORK sicslowpan_driver
#endif /* NETSTACK_CONF_NETWORK */

#ifndef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     ble_ipsp_mac_driver
#endif /* NETSTACK_CONF_MAC */

/* 6LoWPAN */
#define SICSLOWPAN_CONF_MAC_MAX_PAYLOAD         1280
#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_COMPRESSION_HC06
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD   0     /**< Always compress IPv6 packets. */
#define SICSLOWPAN_CONF_FRAG                    0     /**< We don't use 6LoWPAN fragmentation as IPSP takes care of that for us.*/
#define SICSLOWPAN_FRAMER_HDRLEN                0     /**< Use fixed header len rather than framer.length() function */

/* Packet buffer */
#define PACKETBUF_CONF_SIZE                     1280  /**< Required IPv6 MTU size */
/** @} */

/**
 * \name BLE configuration
 * @{
 */
#ifndef DEVICE_NAME
#define DEVICE_NAME "Contiki DWM1001"  /**< Device name used in BLE undirected advertisement. */
#endif
/**
 * @}
 */

/**
 * \name IPv6 network buffer configuration
 *
 * @{
 */
/* Don't let contiki-default-conf.h decide if we are an IPv6 build */
#ifndef NETSTACK_CONF_WITH_IPV6
#define NETSTACK_CONF_WITH_IPV6              0
#endif

#if NETSTACK_CONF_WITH_IPV6
/*---------------------------------------------------------------------------*/
/* Addresses, Sizes and Interfaces */
#define LINKADDR_CONF_SIZE                   8
#define UIP_CONF_LL_802154                   1
#define UIP_CONF_LLH_LEN                     0

/* The size of the uIP main buffer */
#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE              1280
#endif

/* ND and Routing */
#define UIP_CONF_ROUTER                      0 /**< BLE master role, which allows for routing, isn't supported. */
#define UIP_CONF_ND6_SEND_NS                 1
#define UIP_CONF_IP_FORWARD                  0 /**< No packet forwarding. */

#define UIP_CONF_ND6_REACHABLE_TIME     600000
#define UIP_CONF_ND6_RETRANS_TIMER       10000

#ifndef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS        20
#endif

#ifndef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES                 20
#endif

#ifndef UIP_CONF_TCP
#define UIP_CONF_TCP                         1
#endif

#ifndef UIP_CONF_TCP_MSS
#define UIP_CONF_TCP_MSS                    64
#endif

#define UIP_CONF_UDP                         1
#define UIP_CONF_UDP_CHECKSUMS               1
#define UIP_CONF_ICMP6                       1
#endif /* NETSTACK_CONF_WITH_IPV6 */
/** @} */
