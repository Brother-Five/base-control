#include "bsp.h"

mbx_arr_declare(MBX_rx0_ctrl,2,50);
mbx_arr_declare(MBX_rx1_ctrl,2,50);

#ifdef USE_CAN1
CAN_T CAN1_FIFO;
#endif
#ifdef USE_CAN2
CAN_T CAN2_FIFO;
#endif


void CANCheckFifo(void)
{
//#ifdef USE_CAN1
//  u1_printf("1:%d\r\n",CAN1_FIFO.usTxCount);
//#endif
//#ifdef USE_CAN2
//	u1_printf("2:%d\r\n",CAN2_FIFO.usTxCount);
//#endif
	u1_printf("1:%d\t2:%d\r\n",CAN1_FIFO.usTxCount,CAN2_FIFO.usTxCount);
}

/**
 * [bsp_InitCAN ����CANӲ����CanFifo�ĳ�ʼ��]
 */
void bsp_InitCAN(void)
{
	CanFifoInit();
	InitHardCAN();		/* ����CAN��Ӳ������*/
}

/**
 * [CanFifoInit CanFifo�ĳ�ʼ��]
 */
void CanFifoInit(void)
{
#ifdef USE_CAN1
	CAN1_FIFO.usTxWrite = 0;
	CAN1_FIFO.usTxRead = 0;
	CAN1_FIFO.usTxCount = 0;
	CAN1_FIFO.usTxRead = 0;
	CAN1_FIFO.MailBoxFlag = 0xC0;
#endif
#ifdef USE_CAN2
	CAN2_FIFO.usTxWrite = 0;
	CAN2_FIFO.usTxRead = 0;
	CAN2_FIFO.usTxCount = 0;
	CAN2_FIFO.usTxRead = 0;
	CAN2_FIFO.MailBoxFlag = 0xC0;
#endif
}

/**
 * [InitHardCAN ����CANӲ��]
 */
void InitHardCAN(void)
{
  GPIO_InitTypeDef     GPIO_InitStructure;
  CAN_InitTypeDef        CAN_InitStructure;

#ifdef USE_CAN1
  /* ʹ��CANʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  /* CAN GPIOs ����*/
  /* ʹ��GPIOʱ�� */
  RCC_AHB1PeriphClockCmd(CAN1_GPIO_TX_CLK|CAN1_GPIO_RX_CLK, ENABLE);

  /* ����ӳ��ΪCAN����  */
  GPIO_PinAFConfig(CAN1_GPIO_RX_PORT, CAN1_RX_SOURCE, GPIO_AF_CAN1);
  GPIO_PinAFConfig(CAN1_GPIO_TX_PORT, CAN1_TX_SOURCE, GPIO_AF_CAN1);

  /* ���� CAN RX �� TX ���� */
  GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN1_GPIO_RX_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CAN1_TX_PIN;
  GPIO_Init(CAN1_GPIO_TX_PORT, &GPIO_InitStructure);
  /* ��λCAN�Ĵ��� */
  CAN_DeInit(CAN1);

  CAN_InitStructure.CAN_TTCM = _CAN1_TTCM_;     /* ��ֹʱ�䴥��ģʽ��������ʱ���), T  */
  CAN_InitStructure.CAN_ABOM = _CAN1_ABOM_;     /* ��ֹ�Զ����߹رչ��� */
  CAN_InitStructure.CAN_AWUM = _CAN1_AWUM_;     /* ��ֹ�Զ�����ģʽ */
  CAN_InitStructure.CAN_NART = _CAN1_NART_;     /* ��ֹ�ٲö�ʧ��������Զ��ش����� */
  CAN_InitStructure.CAN_RFLM = _CAN1_RFLM_;     /* ��ֹ����FIFO����ģʽ */
  CAN_InitStructure.CAN_TXFP = _CAN1_TXFP_;     /* ��ֹ����FIFO���ȼ� */
  CAN_InitStructure.CAN_Mode = _CAN1_Mode_; /* ����CANΪ��������ģʽ */

  /*CAN ������ = RCC_APB1Periph_CAN1 / Prescaler / (SJW + BS1 + BS2);*/
  CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
   CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
   CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
   CAN_InitStructure.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
  CAN_Init(CAN1, &CAN_InitStructure);

  // can_nvic_config(CAN1_T);
  can_nvic_config(CAN1_R0);
  can_nvic_config(CAN1_R1);
#endif

#ifdef USE_CAN2
  /* ʹ��CANʱ��RCC_APB1Periph_CAN1 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2|RCC_APB1Periph_CAN1, ENABLE);
  /* CAN GPIOs ����*/
  /* ʹ��GPIOʱ�� */
  RCC_AHB1PeriphClockCmd(CAN2_GPIO_TX_CLK|CAN2_GPIO_RX_CLK, ENABLE);

  /* ����ӳ��ΪCAN����  */
  GPIO_PinAFConfig(CAN2_GPIO_RX_PORT, CAN2_RX_SOURCE, GPIO_AF_CAN2);
  GPIO_PinAFConfig(CAN2_GPIO_TX_PORT, CAN2_TX_SOURCE, GPIO_AF_CAN2);

  /* ���� CAN RX �� TX ���� */
  GPIO_InitStructure.GPIO_Pin = CAN2_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN2_GPIO_RX_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CAN2_TX_PIN;
  GPIO_Init(CAN2_GPIO_TX_PORT, &GPIO_InitStructure);
  /* ��λCAN�Ĵ��� */
  CAN_DeInit(CAN2);

  CAN_InitStructure.CAN_TTCM = _CAN2_TTCM_;     /* ��ֹʱ�䴥��ģʽ��������ʱ���), T  */
  CAN_InitStructure.CAN_ABOM = _CAN2_ABOM_;     /* ��ֹ�Զ����߹رչ��� */
  CAN_InitStructure.CAN_AWUM = _CAN2_AWUM_;     /* ��ֹ�Զ�����ģʽ */
  CAN_InitStructure.CAN_NART = _CAN2_NART_;     /* ��ֹ�ٲö�ʧ��������Զ��ش����� */
  CAN_InitStructure.CAN_RFLM = _CAN2_RFLM_;     /* ��ֹ����FIFO����ģʽ */
  CAN_InitStructure.CAN_TXFP = _CAN2_TXFP_;     /* ��ֹ����FIFO���ȼ� */
  CAN_InitStructure.CAN_Mode = _CAN2_Mode_; /* ����CANΪ��������ģʽ */

  /*CAN ������ = RCC_APB1Periph_CAN2 / Prescaler / (SJW + BS1 + BS2);*/
  CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
  CAN_InitStructure.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
  CAN_Init(CAN2, &CAN_InitStructure);

  // can_nvic_config(CAN2_T);
  can_nvic_config(CAN2_R0);
  can_nvic_config(CAN2_R1);
#endif

  CAN_SlaveStartBank(CAN2_StartBank) ;//CAN2Filter����ʼ�洢��
}


/**
 * [can_filter_Mask_config ����CAN��ɸѡ��]
 * @param object_para [|���|FIFOx|ID����|֡����|ID|]
 * @param Id          [ID]
 * @param MaskId      [����λ(0x3ff,0x1FFFFFFF)]
 * @brief             [ɸѡ��:|���|FIFOx|ID����|֡����|ID|����λ(0x3ff,0x1FFFFFFF)|]
 */
void can_filter_Mask_config(uint8_t object_para,uint32_t Id,uint32_t MaskId)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  if( (object_para&0x02))//�ж��Ƿ�Ϊ����֡
  {
    CAN_FilterInitStructure.CAN_FilterIdHigh         = Id<<3<<16;             /* �����ID�ĸ�16bit */
    CAN_FilterInitStructure.CAN_FilterIdLow          = Id<<3| ((object_para&0x03)<<1);    /* �����ID�ĵ�16bit */
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = MaskId<<3<<16;             /* ID����ֵ��16bit */
    CAN_FilterInitStructure.CAN_FilterMaskIdLow      = MaskId<<3| ((object_para&0x03)<<1);; /* ID����ֵ��16bit */
  }
  else
  {
    CAN_FilterInitStructure.CAN_FilterIdHigh         = Id<<5;             /* �����ID�ĸ�16bit */
    CAN_FilterInitStructure.CAN_FilterIdLow          = ((object_para&0x03)<<1);   /* �����ID�ĵ�16bit */
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = MaskId<<5;             /* ID����ֵ��16bit */
    CAN_FilterInitStructure.CAN_FilterMaskIdLow      = ((object_para&0x03)<<1);;  /* ID����ֵ��16bit */
  }


  CAN_FilterInitStructure.CAN_FilterNumber     = object_para>>3;              /* �˲�����ţ�0-13����14���˲��� */
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = (object_para>>2)&0x01;       /* �˲�����FIFO 0 */
  CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;                /* ʹ���˲��� */
  CAN_FilterInitStructure.CAN_FilterMode       = CAN_FilterMode_IdMask;       /* �˲���ģʽ������ID����ģʽ */
  CAN_FilterInitStructure.CAN_FilterScale      = CAN_FilterScale_32bit;       /* 32λ�˲� */
  CAN_FilterInit(&CAN_FilterInitStructure);
}

/**
 * [can_nvic_config ����CAN���ж�]
 * @param _temp [ CAN1_T CAN1_R0 CAN1_R1  CAN2_T CAN2_R0  CAN2_R1 ]
 */
void can_nvic_config(CAN_INT_E _temp)//,void * _pcallback
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = _temp;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  switch (_temp)
  {
#ifdef USE_CAN1
    //��Ϊ����FIFO�����ͣ�����Ҫ�õ������ж�
     case CAN1_T :NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_CAN1_T_Pre;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_CAN1_T_Sub;
           NVIC_Init(&NVIC_InitStructure);

    case CAN1_R0:NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_CAN1_R0_Pre;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_CAN1_R0_Sub;
           NVIC_Init(&NVIC_InitStructure);
           CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    case CAN1_R1:NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_CAN1_R1_Pre;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_CAN1_R1_Sub;
           NVIC_Init(&NVIC_InitStructure);
           CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);

#endif

#ifdef USE_CAN2
    //��Ϊ����FIFO�����ͣ�����Ҫ�õ������ж�
    case CAN2_T :NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_CAN2_T_Pre;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_CAN2_T_Sub;
           NVIC_Init(&NVIC_InitStructure);
           CAN_ITConfig(CAN2, CAN_IT_TME, ENABLE);
           CAN_ClearFlag(CAN2, CAN_IT_TME);

    case CAN2_R0:NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_CAN2_R0_Pre;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_CAN2_R0_Sub;
           NVIC_Init(&NVIC_InitStructure);
           CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

    case CAN2_R1:NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Priority_CAN2_R1_Pre;
           NVIC_InitStructure.NVIC_IRQChannelSubPriority        = Priority_CAN2_R1_Sub;
           NVIC_Init(&NVIC_InitStructure);
           CAN_ITConfig(CAN2, CAN_IT_FMP1, ENABLE);
#endif
    default:break;
  }
}


/**
 * [PUSH_CAN_FIFO ���ݵ�ѹ��FIFO]
 * @param CANx    [CAN1 CAN2]
 * @param Msgtemp [CanTxMsg���͵�����ָ��]
 */
void PUSH_CAN_FIFO(CAN_TypeDef* CANx,CanTxMsg *Msgtemp)
{
	CAN_T* _pCAN_FIFO;
	if(CANx == CAN1)
#ifdef USE_CAN1
		_pCAN_FIFO = &CAN1_FIFO;
#else
	;
#endif
	else if(CANx == CAN2)
#ifdef USE_CAN2
		_pCAN_FIFO = &CAN2_FIFO;
#else
	;
#endif
	else return;
	while (_pCAN_FIFO->usTxCount >= CAN_FIFO_SIZE-1)//�ȴ����ݴﵽ�߽磬�ȴ�������
	{
		CAN_FIFO_IRQ();
		u1_printf("���\r\n");
	}
	_pCAN_FIFO->_CanBuf[_pCAN_FIFO->usTxWrite] = *Msgtemp;
	_pCAN_FIFO->usTxCount++;
	if (++_pCAN_FIFO->usTxWrite  >= CAN_FIFO_SIZE)
	{
		_pCAN_FIFO->usTxWrite = 0;
	}
	CAN_FIFO_IRQ();
}

/**
 * [POP_CAN_FIFO ���ݴ�FIFO��ȡ��]
 * @param  CANx    [CAN1 CAN2]
 * @param  Msgtemp [CanTxMsg���͵�����ָ��]
 * @return         [1 Ϊ����ȡ�� , 0 Ϊδ����ȡ��]
 */
uint8_t POP_CAN_FIFO(CAN_TypeDef* CANx,CanTxMsg *Msgtemp)
{
	CAN_T* _pCAN_FIFO;
	if(CANx == CAN1)
#ifdef USE_CAN1
		_pCAN_FIFO = &CAN1_FIFO;
#else
	;
#endif
	else if(CANx == CAN2)
#ifdef USE_CAN2
		_pCAN_FIFO = &CAN2_FIFO;
#else
	;
#endif
	else return 0;
	if (_pCAN_FIFO->usTxRead == _pCAN_FIFO->usTxWrite)
	{
		return 0;
	}
	else
	{
		*Msgtemp = _pCAN_FIFO->_CanBuf[_pCAN_FIFO->usTxRead];
		_pCAN_FIFO->usTxCount--;
		if (++_pCAN_FIFO->usTxRead >= CAN_FIFO_SIZE)
		{
			_pCAN_FIFO->usTxRead = 0;
		}
		return 1;
	}
}

/**
 * [CAN_FIFO_ToSend CAN��FIFO��ѯ����]
 * @param CANx [CAN1 CAN2]
 */
void CAN_FIFO_ToSend(CAN_TypeDef* CANx)
{
	uint8_t i;
	CAN_T* _pCAN_FIFO;
	if(CANx == CAN1)
#ifdef USE_CAN1
		_pCAN_FIFO = &CAN1_FIFO;
#else
	;
#endif
	else if(CANx == CAN2)
#ifdef USE_CAN2
		_pCAN_FIFO = &CAN2_FIFO;
#else
	;
#endif
	else return;

	// else if
	if(_pCAN_FIFO->usTxCount)
	{
		// printf("x:%d\tcnt:%d\tflag:%x\r\n",(int)_pCAN_FIFO,_pCAN_FIFO->usTxCount,_pCAN_FIFO->MailBoxFlag);
		// printf("usTxCount:%d\r\n",_pCAN_FIFO->usTxCount);
		if(_pCAN_FIFO->MailBoxFlag&0x80)
		{
			// printf("ALL��%x\r\n",_pCAN_FIFO->MailBoxFlag);
			_pCAN_FIFO->MailBoxFlag&=0x7f;//���ȫ������Ϊ�յ�״̬
			for(i=0;i<3;i++)
			{
				if(POP_CAN_FIFO(CANx,&_pCAN_FIFO->_tmeptosend[i]))
					CAN_TransmitForMailbox(CANx,i,&_pCAN_FIFO->_tmeptosend[i]);
				else break;
			}
		// 	printf("ALL OVER��%x\r\n",_pCAN_FIFO->MailBoxFlag);
		}
		else
		{
			// printf("NOT ALL��%x\r\n",_pCAN_FIFO->MailBoxFlag);
			for(i=0;i<3;i++)
			{
				switch(MyCAN_TransmitStatus(CANx,i))
				{
					case CAN_TxStatus_Ok:
						if(POP_CAN_FIFO(CANx,&_pCAN_FIFO->_tmeptosend[i]))
							//DISABLE_INT();
							CAN_TransmitForMailbox(CANx,i,&_pCAN_FIFO->_tmeptosend[i]);
							//ENABLE_INT();
						_pCAN_FIFO->MailBoxFlag&=0x3f;//���ȫ������Ϊ�յ�״̬,��Ҫ���
											//printf("OK\r\n");
						break;
					case CAN_TxStatus_Pending:////printf("PEN\r\n");
						break;
					case CAN_TxStatus_Failed:
						//DISABLE_INT();
						CAN_TransmitForMailbox(CANx,i,&_pCAN_FIFO->_tmeptosend[i]);//printf("Fail1\r\n");
						//ENABLE_INT();

						break;
					default:
						break;
				}

			}
			// printf("NOT ALL OVER��%x\r\n",_pCAN_FIFO->MailBoxFlag);
		}
	}
	else if( (_pCAN_FIFO->MailBoxFlag&0x40) == 0) //��Ҫ���
	{
		// printf("START CHECK��%x\r\n",_pCAN_FIFO->MailBoxFlag);
		_pCAN_FIFO->MailBoxFlag &=0xf0; //�����4λ�����������������Ŀ
		for(i=0;i<3;i++)
			{
				switch(MyCAN_TransmitStatus(CANx,i))
				{
					case CAN_TxStatus_Ok:_pCAN_FIFO->MailBoxFlag++;//printf("OK\r\n");
						break;
					case CAN_TxStatus_Pending://printf("PENDING\r\n");
						break;
					case CAN_TxStatus_Failed:
						// printf("Fail\r\n");
						//DISABLE_INT();
						CAN_TransmitForMailbox(CANx,i,&_pCAN_FIFO->_tmeptosend[i]);//
						//ENABLE_INT();
						break;
					default:
						break;
				}
			}
//		if(ForOnce == 3)
//			can_nvic_config(CANx_T,(void *)can2_TX);
//		else ForOnce=200;

//		if( (_pCAN_FIFO->MailBoxFlag&0x0f) == 0x0f) _pCAN_FIFO->MailBoxFlag =0xE0;//ȫ������Ϊ�գ�����Ҫ���,���������豸
		if( (_pCAN_FIFO->MailBoxFlag&0x0f) == 3) _pCAN_FIFO->MailBoxFlag =0xC0;//ȫ������Ϊ�գ�����Ҫ���
			// printf("CHECK OVER��%x\r\n",_pCAN_FIFO->MailBoxFlag);

	}
	// printf("x:%d\tcnt:%d\tflag:%x\r\n",(int)_pCAN_FIFO,_pCAN_FIFO->usTxCount,_pCAN_FIFO->MailBoxFlag);
	 // _pCAN_FIFO->MailBoxFlag =0xC0;
//	printf("OVER:%x\r\n",_pCAN_FIFO->MailBoxFlag);
}


/**
 * [MyCAN_TransmitStatus CAN���͵�״̬��ѯ]
 * @param  CANx            [CAN1 CAN2]
 * @param  TransmitMailbox [����ı��]
 * @return                 [���͵�״̬]
 */
uint8_t MyCAN_TransmitStatus(CAN_TypeDef* CANx, uint8_t TransmitMailbox)
{
  uint32_t state = 0;

  /* Check the parameters */
  switch (TransmitMailbox)
  {
    case (0x00):
      state =   CANx->TSR &  (CAN_TSR_RQCP0 | CAN_TSR_TXOK0 | CAN_TSR_TME0);
      break;
    case (0x01):
      state =   CANx->TSR &  (CAN_TSR_RQCP1 | CAN_TSR_TXOK1 | CAN_TSR_TME1);
      break;
    case (0x02):
      state =   CANx->TSR &  (CAN_TSR_RQCP2 | CAN_TSR_TXOK2 | CAN_TSR_TME2);
      break;
    default:
      state = CAN_TxStatus_Failed;
      break;
  }
  switch (state)
  {
      /* transmit pending  */
    case (0x0): state = CAN_TxStatus_Pending;
      break;
      /* transmit failed  */
     case (CAN_TSR_RQCP0 | CAN_TSR_TME0): state = CAN_TxStatus_Failed;
      break;
     case (CAN_TSR_RQCP1 | CAN_TSR_TME1): state = CAN_TxStatus_Failed;
      break;
     case (CAN_TSR_RQCP2 | CAN_TSR_TME2): state = CAN_TxStatus_Failed;
      break;
      /* transmit succeeded  */
    case (CAN_TSR_RQCP0 | CAN_TSR_TXOK0 | CAN_TSR_TME0):state = CAN_TxStatus_Ok;
      break;
    case (CAN_TSR_RQCP1 | CAN_TSR_TXOK1 | CAN_TSR_TME1):state = CAN_TxStatus_Ok;
      break;
    case (CAN_TSR_RQCP2 | CAN_TSR_TXOK2 | CAN_TSR_TME2):state = CAN_TxStatus_Ok;
      break;
      /* transmit first  */
    case CAN_TSR_TME0:state = CAN_TxStatus_Ok;//��ʼTSRֵΪ0x1C000000,����������,ǿ����Ϊ�ϴη��ͳɹ�
      break;
    case CAN_TSR_TME1:state = CAN_TxStatus_Ok;//��ʼTSRֵΪ0x1C000000,����������,ǿ����Ϊ�ϴη��ͳɹ�
      break;
    case CAN_TSR_TME2:state = CAN_TxStatus_Ok;//��ʼTSRֵΪ0x1C000000,����������,ǿ����Ϊ�ϴη��ͳɹ�
	  break;
    default: state = CAN_TxStatus_Failed;
      break;
  }
  return (uint8_t) state;
}

/**
 * [CAN_TransmitForMailbox ����ض����䷢��]
 * @param CANx             [CAN1 CAN2]
 * @param transmit_mailbox [����ı��]
 * @param TxMessage        [CanTxMsg���͵�����ָ��]
 * @brief                  [��Ҫ�ȼ��������Ƿ�Ϊ��,����������ʹ��]
 */
void CAN_TransmitForMailbox(CAN_TypeDef* CANx,uint8_t transmit_mailbox, CanTxMsg* TxMessage)
{
    /* Set up the Id */
//	CANx->sTxMailBox[transmit_mailbox].TIR &= TMIDxR_TXRQ;
    CANx->sTxMailBox[transmit_mailbox].TIR &= 0x00000001;//#define TMIDxR_TXRQ       ((uint32_t)0x00000001) /* Transmit mailbox request */
    if (TxMessage->IDE == CAN_Id_Standard)
    {

      CANx->sTxMailBox[transmit_mailbox].TIR |= ((TxMessage->StdId << 21) | \
                                                  TxMessage->RTR);
    }
    else
    {     assert_param(IS_CAN_EXTID(TxMessage->ExtId));
      CANx->sTxMailBox[transmit_mailbox].TIR |= ((TxMessage->ExtId << 3) | \
                                                  TxMessage->IDE | \
                                                  TxMessage->RTR);
    }
    /* Set up the DLC */
    TxMessage->DLC &= (uint8_t)0x0000000F;
    CANx->sTxMailBox[transmit_mailbox].TDTR &= (uint32_t)0xFFFFFFF0;
    CANx->sTxMailBox[transmit_mailbox].TDTR |= TxMessage->DLC;

    /* Set up the data field */
    CANx->sTxMailBox[transmit_mailbox].TDLR = (((uint32_t)TxMessage->Data[3] << 24) |
                                             ((uint32_t)TxMessage->Data[2] << 16) |
                                             ((uint32_t)TxMessage->Data[1] << 8) |
                                             ((uint32_t)TxMessage->Data[0]));
    CANx->sTxMailBox[transmit_mailbox].TDHR = (((uint32_t)TxMessage->Data[7] << 24) |
                                             ((uint32_t)TxMessage->Data[6] << 16) |
                                             ((uint32_t)TxMessage->Data[5] << 8) |
                                             ((uint32_t)TxMessage->Data[4]));
    /* Request transmission */
//	CANx->sTxMailBox[transmit_mailbox].TIR |= TMIDxR_TXRQ;
    CANx->sTxMailBox[transmit_mailbox].TIR |= 0x00000001;//#define TMIDxR_TXRQ       ((uint32_t)0x00000001) /* Transmit mailbox request */

}

/**
 * [CAN_FIFO_IRQ ��CAN1��CAN2�Ĳ�ѯ���ͺ���ǿ�з��������������]
 */
void CAN_FIFO_IRQ(void)
{
#ifdef USE_CAN1
	CAN_FIFO_ToSend(CAN1);
#endif

#ifdef USE_CAN2
	CAN_FIFO_ToSend(CAN2);
#endif
}


#ifdef USE_CAN1
//��Ϊ����FIFO�����ͣ�����Ҫ�õ������ж�
// void CAN1_TX_IRQHandler(void)
// {
// //	CanRxMsg rx_message;
//     if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET)
// 	{
// 		 CAN1_T_CallBack();
// 	}
// 	CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
// }
void CAN1_RX0_IRQHandler(void)
{
	static CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		if (isr_mbx_check (MBX_rx0_ctrl[0]) != 0)
		{

			isr_mbx_send (MBX_rx0_ctrl[0], &rx_message);
		}
		// printf("%x\r\n",rx_message.StdId);

	}
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}

void CAN1_RX1_IRQHandler(void)
{
	static CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP1)!= RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO1, &rx_message);
		if (isr_mbx_check (MBX_rx0_ctrl[1]) != 0)
		{
			isr_mbx_send (MBX_rx0_ctrl[1], &rx_message);
		}
		// printf("%x\r\n",rx_message.StdId);
	}
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);
}
#endif

#ifdef USE_CAN2
//��Ϊ����FIFO�����ͣ�����Ҫ�õ������ж�
// void CAN2_TX_IRQHandler(void)
// {
// //	CanRxMsg rx_message;
//     if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET)
// 	{
// 		 CAN2_T_CallBack();
// 	}
// 	CAN_ClearITPendingBit(CAN2, CAN_IT_TME);
// }
void CAN2_RX0_IRQHandler(void)
{
	static CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
		if (isr_mbx_check (MBX_rx1_ctrl[0]) != 0)
		{
			isr_mbx_send (MBX_rx1_ctrl[0], &rx_message);
		}
	}
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
}

void CAN2_RX1_IRQHandler(void)
{
	static CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET)
	{
		CAN_Receive(CAN2, CAN_FIFO1, &rx_message);
		if (isr_mbx_check (MBX_rx1_ctrl[1]) != 0)
		{
			isr_mbx_send (MBX_rx1_ctrl[1], &rx_message);
		}
		// printf("%x\r\n",rx_message.StdId);
	}
	CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
}
#endif

void CAN_mailbox_init(uint32_t ctrl)
{
	if (ctrl == 1)
	{
		os_mbx_init(MBX_rx0_ctrl[0], sizeof(MBX_rx0_ctrl[0]));
		os_mbx_init(MBX_rx0_ctrl[1], sizeof(MBX_rx0_ctrl[1]));
	}
	else
	{
		os_mbx_init(MBX_rx1_ctrl[0], sizeof(MBX_rx1_ctrl[0]));
		os_mbx_init(MBX_rx1_ctrl[1], sizeof(MBX_rx1_ctrl[1]));

	}

}


uint8_t CAN_receive (uint32_t ctrl, uint8_t _fifo,CanRxMsg **msg, uint16_t timeout)
{
	if (ctrl == 1)
		return os_mbx_wait(MBX_rx0_ctrl[_fifo], (void **)msg,timeout);
	else
		return os_mbx_wait(MBX_rx1_ctrl[_fifo], (void **)msg,timeout);

}

//uint8_t CAN_send (uint32_t ctrl, CAN_msg *msg, uint16_t timeout)  {
////  msg->type = DATA_FRAME;

////  return (CAN_push (ctrl, msg, timeout));
//}
