/*!
 * @file rv.h
 * @brief Класс РВ (радиовысотомера)
 * @copyright АО ОКБ "Электроавтоматика", НИЦ-1
 * @details
 * #### Номер ВИДК
 *    нет
 * #### Комментарии
 *    нет
 */
#pragma once
#include "system.h"

//! Класс РВ (радиовысотомера)
class rv_t: public system_t {
   public:
   //! Объявление наследуемых методов класса @a system_t
   SYSTEM_DECLARATIONS(rv_t)

   //! Идентификатор
   const uchar id = ID_RV;

   //! Краткое наименование системы
   par_list_t *name = create_par_list(id, "РВ");

   //! Каналы связи включены
   par_bool_t *on = create_par_bool(id, nullptr, "Каналы связи включены", PAR_BOOL_BUTTON_SMALL, true);

   //! Отступ
   par_empty_t *empty_1 = create_par_empty(id);

   //! Заголовок
   par_title_t *title_1 = create_par_title(id, "Выходные параметры");

   //! Радиовысота
   par_double_t *out_altitude = create_par_double(id, nullptr, "Радиовысота", 0., 0., 1500., "м", PAR_CAN_BE_INVALID);
   
   //! Исправность РВ (только для автономного режима)
   par_bool_t *out_healthy = create_par_bool(id, nullptr, "Исправность РВ", PAR_BOOL_BUTTON_LARGE, true);

   //! РВ готов (только для автономного режима)
   par_bool_t *out_ready = create_par_bool(id, nullptr, "РВ готов", PAR_BOOL_BUTTON_LARGE, true);

   //! Контроль РВ
   par_bool_t *out_test = create_par_bool(id, nullptr, "Контроль РВ");

   //! Режим
   typedef enum out_mode_t: uint {
      OUT_MODE_NO_DATA, //!< Поиск
      OUT_MODE_NORMAL,  //!< Слежение
      OUT_MODE_TEST,    //!< Контроль
      OUT_MODE_SILENCE, //!< Радиомолчание
      OUT_MODE_OFF,     //!< Отключен
      OUT_MODE_OFF_RK   //!< Отключен по РК «H>1500»
   } out_mode_t;

   //! Режим
   par_enum_read_only_t *out_mode = create_par_enum_read_only(id, nullptr, "Режим", OUT_MODE_NORMAL, "0 поиск;1 слежение;2 контроль;3 радиомолчание;4 отключен;5 отключен по РК «H>10000м»");

   //! Отступ
   par_empty_t *empty_2 = create_par_empty(id);

   //! Заголовок
   par_title_t *title_2 = create_par_title(id, "Входные параметры");

   //! Радиомолчание РВ
   par_bool_read_only_t *in_radio_silence = create_par_bool_read_only(id, nullptr, "Отключение передатчика (радиомолчание)");

   //! Отключение РВ (H>1500м) (только для автономного режима)
   par_bool_read_only_t *in_altitude_higher_1500 = create_par_bool_read_only(id, nullptr, "Отключение РВ (H>1500м)");

   //! Контроль РВ (только для автономного режима)
   par_bool_read_only_t *in_test = create_par_bool_read_only(id, nullptr, "Контроль РВ");

   static const md_frequency_t channels_frequency = MD_FREQUENCY_50; //!< Частота каналов выдачи
   static const size_t out_words_count = 1;                          //!< Количество выходных слов данных
   static const uchar out_words_addresses[out_words_count];          //!< Адреса выходных слов данных (неинвертированные)
   uint out_words[out_words_count];                                  //!< Выходные слова данных




	//определение функции класса
   static void out_test_change_handler(par_t *par, const par_change_flags_t change_flags);//Обрабатывает событие изменения параметра "Контроль РВ"
   void pack();//упаковка
   void update();//Обновляет данные бортовой системы
	rv_t();//конструктор класса 
	~rv_t();//деструктор класса
	
   private:
   const double out_altitude_test_value; //!< Тестовое значение радиовысоты
   
  
   
   
};

