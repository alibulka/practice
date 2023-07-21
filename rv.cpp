/*!
 * @file rv.cpp
 * @brief Класс РВ (радиовысотомера)
 * @copyright АО ОКБ "Электроавтоматика", НИЦ-1
 * @details
 * #### Файл-спецификация
 *    rv.h
 * #### Номер ВИДК
 *    нет
 * #### Комментарии
 *    нет
 */
#include "rv.h"
#include "all.h"



//! Адреса выходных слов данных (неинвертированные)
const uchar rv_t::out_words_addresses[out_words_count] = { 0340 };

//! Обрабатывает событие изменения параметра "Контроль РВ"
static par_change_handler_t out_test_change_handler;

/*!
 * Класс РВ (радиовысотомера, конструктор)
 */
rv_t::rv_t():
   out_words {},
   out_altitude_test_value(frandom(14.5, 19.5)) {
   out_test->add_change_handler(out_test_change_handler);
}

/*!
 * Класс РВ (радиовысотомера, деструктор)
 */
rv_t::~rv_t() {
}



/*!
 * Обновляет данные бортовой системы
 */
void rv_t::update() {
   // Статус системы в корневом списке параметров (цвет ссылки перехода на страницу системы)
   name->set_list_status(on->get_value() ? PAR_LIST_STATUS_VALID : PAR_LIST_STATUS_INVALID);

   // Тест-контроль (прием входной команды в автономном режиме имитатора)
   if (settings.imit_mode->get_value() == IMIT_MODE_STANDALONE || settings.imit_mode->get_value() == IMIT_MODE_MK31) {
      out_test->set_status(PAR_STATUS_AUTO);
      out_test->set_value(in_test->get_value());
   }

   // Использование данных полетной модели
   if (flight_model.on->get_value()) {
      out_altitude->set_status(PAR_STATUS_AUTO);
      out_altitude->set_value(flimit(flight_model.out.rv_altitude, out_altitude->get_min_value(), out_altitude->get_max_value()));
      out_altitude->set_valid(flight_model.out.rv_altitude_valid);
      if (settings.imit_mode->get_value() == IMIT_MODE_STANDALONE || settings.imit_mode->get_value() == IMIT_MODE_MK31) {
         out_ready->set_status(PAR_STATUS_AUTO);
         out_ready->set_value(flight_model.out.rv_altitude_valid);
      }
   }

   // Тест-контроль (выполнение)
   if (out_test->get_value()) {
      out_altitude->set_status(PAR_STATUS_TEST);
      out_altitude->set_value(out_altitude_test_value);
   }

   // Определение текущего режима РВ
   if (!on->get_value())
      out_mode->set_value(OUT_MODE_OFF);
   else if (out_test->get_value())
      out_mode->set_value(OUT_MODE_TEST);
   else if ((settings.imit_mode->get_value() == IMIT_MODE_STANDALONE || settings.imit_mode->get_value() == IMIT_MODE_MK31) && in_altitude_higher_10000->get_value())
      out_mode->set_value(OUT_MODE_OFF_RK);
   else if ((settings.imit_mode->get_value() == IMIT_MODE_STANDALONE || settings.imit_mode->get_value() == IMIT_MODE_MK31) && in_radio_silence->get_value())
      out_mode->set_value(OUT_MODE_SILENCE);
   else if (out_altitude->get_valid())
      out_mode->set_value(OUT_MODE_NORMAL);
   else
      out_mode->set_value(OUT_MODE_NO_DATA);

   // Параметры РВ, недоступные на стенде СК-31
   if (settings.imit_mode->get_value() == IMIT_MODE_SK31) {
      out_healthy->set_status(PAR_STATUS_DISABLED);
      out_ready->set_status(PAR_STATUS_DISABLED);
      in_test->set_status(PAR_STATUS_DISABLED);
      in_altitude_higher_1500->set_status(PAR_STATUS_DISABLED);
      out_healthy->set_value(false);
      out_ready->set_value(false);
   }
}

/*!
 * Выполняет упаковку
 */
void rv_t::pack() {
   uint *w = out_words;

   // СД 340 Радиовысота
   *w++ =
      (out_altitude->get_status() == PAR_STATUS_TEST ? MTX_01 : out_altitude->get_valid() ? MTX_00 : MTX_10) |
      pack_u(out_altitude->get_value(), 8192., 26, 12);

   // Проверка количества записанных слов
   assert(w - out_words == out_words_count);

   // Запись адресов слов и 32-го разряда
   md_set_addresses(out_words, out_words_addresses, out_words_count, 1);
}

/*!
 * Обрабатывает событие изменения параметра "Контроль РВ"
 * @param par Параметр-источник события
 * @param change_flags Признаки события
 */
static void out_test_change_handler(par_t *par, const par_change_flags_t change_flags) {
   (void)par;
   if ((change_flags & PAR_VALUE_CHANGED) != 0) {
      // Сохранение и восстановление значений, установленных до тест-контроля
      if (rv.out_test->get_value()) {
         rv.out_altitude->save();
      } else {
         rv.out_altitude->load();
      }
   }
}
