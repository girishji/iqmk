#include "eeprom.h"

uint8_t eeprom_read_byte(const uint8_t *addr) {
    return 0;
}

void eeprom_write_byte(uint8_t *addr, uint8_t value) {
}

uint16_t eeprom_read_word(const uint16_t *addr) {
    return 0;
}

uint32_t eeprom_read_dword(const uint32_t *addr) {
    return 0;
}

void eeprom_read_block(void *buf, const void *addr, size_t len) {
}

void eeprom_write_word(uint16_t *addr, uint16_t value) {
}

void eeprom_write_dword(uint32_t *addr, uint32_t value) {
}

void eeprom_write_block(const void *buf, void *addr, size_t len) {
}

void eeprom_update_byte(uint8_t *addr, uint8_t value) {
}

void eeprom_update_word(uint16_t *addr, uint16_t value) {
}

void eeprom_update_dword(uint32_t *addr, uint32_t value) {
}

void eeprom_update_block(const void *buf, void *addr, size_t len) {
}
