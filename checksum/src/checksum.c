#include "inttypes.h"
#include "stdio.h"

uint16_t compute_checksum(uint8_t *udp_packet, uint32_t len) {
  // Note that the actual checksum used in the RFC is different (needs IP
  // header). Here, we use the checksum of UDP header + data only.
  uint32_t sum = 0;
  uint16_t *p = (uint16_t *)udp_packet;

  // Sum all 16-bit bytes in the data using ones-complement addition
  for (uint32_t i = 0; i < len - 2; i += 2) {
    // (skip the checksum field, the 6th and 7th byte).
    if (i == 6) {
      p += 2;
    }
    sum += *p++;
  }

  // Add the last byte if it is odd
  if (len % 2) {
    sum += *p;
  }

  // Add the carry bits
  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }

  // Invert the sum
  sum = ~sum;
  return sum;
}

int main(void) {
  uint8_t udp_packet[21] = {0x0B, 0xB8, // Source Port (3000),
                            0x00, 0x50, // Destination Port (80),
                            0x00, 0x15, // Length (8 header + 13 data = 21 byte)
                            0x00, 0x00, // Checksum - needs to be calculated
                            // Data Portion
                            0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x43, 0x50,
                            0x53, 0x33, 0x37, 0x33, 0x21};

  uint16_t checksum = compute_checksum(udp_packet, 21);
  // Add the checksum to the udp_packet
  udp_packet[6] = (checksum >> 8) & 0xff;
  udp_packet[7] = checksum & 0xff;

  // simulate a network packet loss (via a bit flip)
  udp_packet[17] ^= 0x02;

  // calculate the checksum again
  checksum = compute_checksum(udp_packet, 21);

  // check if the checksum is correct
  if (checksum == ((udp_packet[6] << 8) | udp_packet[7])) {
    printf("Checksum is correct\n");
  } else {
    printf("Checksum is incorrect: Expected 0x%02x%02x but got 0x%02x%02x\n",
           udp_packet[6], udp_packet[7], (checksum >> 8) & 0xff,
           checksum & 0xff);
  }

  // Print data portion as char*
  printf("Data: ");
  for (uint32_t i = 8; i < 21; i++) {
    printf("%c", udp_packet[i]);
  }
  printf("\n");

  return 0;
}
