#include "hstatusbar.h"


/***************** Static variables: ***********************/
static HWidgetOperation		s_WidgetOper;
static HStatusBarOperation	s_StatusBarOper;


/***************** virtual functions: ***********************/
static HClass UI_StatusBar_GetClass(HWidget *p_this)
{
	return CLASS_STATUS_BAR;
}


/***************** Static functions: ***********************/
static void UI_StatusBar_SetKey(HStatusBar *p_this, ENUM_SOFTKEY_TYPE e_type, VMWSTR text, VMINT color, void (*pf_action)(HWidget*,void*))
{
	HContainer * const p_container = (HContainer*)p_this;

	if( NULL == p_this)			return;
	if( e_type >= SOFTKEY_MAX )	return;


	if( p_this->p_Btns[e_type] != NULL )
	{	
		p_container->p_container_ops->remove_widget( p_container, (HWidget*)(p_this->p_Btns[e_type]) );
		p_this->p_Btns[e_type] = NULL;
	}

	p_this->p_Btns[e_type] = hpushbutton_new();

	if( NULL == p_this->p_Btns[e_type] )	// 分配失败
		return;

	// Set the properties of the button
	p_this->p_Btns[e_type]->set_text( p_this->p_Btns[e_type], text );
	p_this->p_Btns[e_type]->set_click_action( p_this->p_Btns[e_type], pf_action );

	// Set the position
	switch(e_type)
	{
	case SOFTKEY_LEFT:
		break;

	case SOFTKEY_MID:
		break;

	case SOFTKEY_RIGHT:
		break;
	}

	// Add the button to the container
	p_container->p_container_ops->add_widget( p_container, (HWidget*)(p_this->p_Btns[e_type]) );
}


static void UI_StatusBar_Clean(HStatusBar *p_this)
{
	if( NULL == p_this )
		return;

	// 清理父类 HContainer
	// 没有接口
}


static HStatusBarOperation* UI_StatusBar_Get_Oper()
{
	static int i_Flag = FALSE;

	if( FALSE == i_Flag )
	{
		i_Flag = TRUE;

		s_StatusBarOper.set_soft_key = UI_StatusBar_SetKey;
		s_StatusBarOper.clean		 = UI_StatusBar_Clean;
	}

	return &s_StatusBarOper;
}


static HWidgetOperation* UI_StatusBar_Get_WidgetOper(HWidgetOperation *p_Src)
{
	static int i_Flag = FALSE;

	if( FALSE == i_Flag )
	{
		i_Flag = TRUE;

		s_WidgetOper = *p_Src;

		s_WidgetOper.get_class = UI_StatusBar_GetClass;
	}

	return &s_WidgetOper;
}


/***************** External functions: ***********************/

void hstatusbar_init(HStatusBar* p_this)
{
	HWidget *p_widget = (HWidget*)p_this;
	int i = 0;

	if ( NULL == p_this ) return;

	// initialize its base
	hcontainer_init( &(p_this->base) );			

	// 重新设置基类 HWidget 的 operation 指针
	p_widget->p_widget_ops	= UI_StatusBar_Get_WidgetOper( p_widget->p_widget_ops );

	// get the operation pointer
	p_this->p_oper = UI_StatusBar_Get_Oper();	

	for( i = 0; i < SOFTKEY_MAX; i++ )
	{	
		p_this->p_Btns[i] = NULL;
	}
}
