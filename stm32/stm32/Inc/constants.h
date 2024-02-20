
typedef enum {
    LR1110_TCXO_CTRL_1_6V = 0x00,  //!< Supply voltage = 1.6v
    LR1110_TCXO_CTRL_1_7V = 0x01,  //!< Supply voltage = 1.7v
    LR1110_TCXO_CTRL_1_8V = 0x02,  //!< Supply voltage = 1.8v
    LR1110_TCXO_CTRL_2_2V = 0x03,  //!< Supply voltage = 2.2v
    LR1110_TCXO_CTRL_2_4V = 0x04,  //!< Supply voltage = 2.4v
    LR1110_TCXO_CTRL_2_7V = 0x05,  //!< Supply voltage = 2.7v
    LR1110_TCXO_CTRL_3_0V = 0x06,  //!< Supply voltage = 3.0v
    LR1110_TCXO_CTRL_3_3V = 0x07,  //!< Supply voltage = 3.3v
} lr1110_tcxo_supply_voltage_t;

typedef struct configuration {
    GPIO_TypeDef* port;
    uint32_t      pin;
} gpio_t;

typedef struct hal_gpio_irq_s {
    gpio_t irq1;
    void*                context;
    void ( *callback )( void* context );
} hal_gpio_irq_t;

typedef struct {
    SPI_TypeDef*    spi;
    gpio_t          nss;
    gpio_t          reset;
    hal_gpio_irq_t  event;
    gpio_t          busy;
} radio_t;