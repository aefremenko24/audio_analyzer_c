//
// Created by Arthur Efremenko on 4/11/25.
//

#include "dispatch.h"
#include <stdlib.h>
#include <string.h>

void update_global_buffer(float *in, float *out)
{
  memcpy(global_input_buffer, in, sizeof(global_input_buffer));
  memcpy(global_output_buffer, out, sizeof(global_output_buffer));

}