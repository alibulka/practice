/*!
 * @file rv.h
 * @brief ����� �� (���������������)
 * @copyright �� ��� "�����������������", ���-1
 * @details
 * #### ����� ����
 *    ���
 * #### �����������
 *    ���
 */
#pragma once
#include "system.h"

//! ����� �� (���������������)
class rv_t: public system_t {
   public:
   //! ���������� ����������� ������� ������ @a system_t
   SYSTEM_DECLARATIONS(rv_t)

   //! �������������
   const uchar id = ID_RV;

   //! ������� ������������ �������
   par_list_t *name = create_par_list(id, "��");

   //! ������ ����� ��������
   par_bool_t *on = create_par_bool(id, nullptr, "������ ����� ��������", PAR_BOOL_BUTTON_SMALL, true);

   //! ������
   par_empty_t *empty_1 = create_par_empty(id);

   //! ���������
   par_title_t *title_1 = create_par_title(id, "�������� ���������");

   //! �����������
   par_double_t *out_altitude = create_par_double(id, nullptr, "�����������", 0., 0., 1500., "�", PAR_CAN_BE_INVALID);
   
   //! ����������� �� (������ ��� ����������� ������)
   par_bool_t *out_healthy = create_par_bool(id, nullptr, "����������� ��", PAR_BOOL_BUTTON_LARGE, true);

   //! �� ����� (������ ��� ����������� ������)
   par_bool_t *out_ready = create_par_bool(id, nullptr, "�� �����", PAR_BOOL_BUTTON_LARGE, true);

   //! �������� ��
   par_bool_t *out_test = create_par_bool(id, nullptr, "�������� ��");

   //! �����
   typedef enum out_mode_t: uint {
      OUT_MODE_NO_DATA, //!< �����
      OUT_MODE_NORMAL,  //!< ��������
      OUT_MODE_TEST,    //!< ��������
      OUT_MODE_SILENCE, //!< �������������
      OUT_MODE_OFF,     //!< ��������
      OUT_MODE_OFF_RK   //!< �������� �� �� �H>1500�
   } out_mode_t;

   //! �����
   par_enum_read_only_t *out_mode = create_par_enum_read_only(id, nullptr, "�����", OUT_MODE_NORMAL, "0 �����;1 ��������;2 ��������;3 �������������;4 ��������;5 �������� �� �� �H>10000�");

   //! ������
   par_empty_t *empty_2 = create_par_empty(id);

   //! ���������
   par_title_t *title_2 = create_par_title(id, "������� ���������");

   //! ������������� ��
   par_bool_read_only_t *in_radio_silence = create_par_bool_read_only(id, nullptr, "���������� ����������� (�������������)");

   //! ���������� �� (H>1500�) (������ ��� ����������� ������)
   par_bool_read_only_t *in_altitude_higher_1500 = create_par_bool_read_only(id, nullptr, "���������� �� (H>1500�)");

   //! �������� �� (������ ��� ����������� ������)
   par_bool_read_only_t *in_test = create_par_bool_read_only(id, nullptr, "�������� ��");

   static const md_frequency_t channels_frequency = MD_FREQUENCY_50; //!< ������� ������� ������
   static const size_t out_words_count = 1;                          //!< ���������� �������� ���� ������
   static const uchar out_words_addresses[out_words_count];          //!< ������ �������� ���� ������ (�����������������)
   uint out_words[out_words_count];                                  //!< �������� ����� ������




	//����������� ������� ������
   static void out_test_change_handler(par_t *par, const par_change_flags_t change_flags);//������������ ������� ��������� ��������� "�������� ��"
   void pack();//��������
   void update();//��������� ������ �������� �������
	rv_t();//����������� ������ 
	~rv_t();//���������� ������
	
   private:
   const double out_altitude_test_value; //!< �������� �������� �����������
   
  
   
   
};

