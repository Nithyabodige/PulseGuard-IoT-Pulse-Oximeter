#include <LPC21xx.h>
#include "project_pg.h"
#include "all_defines.h"
#define MAX30102_ADDR 0x57
#define BUFFER_SIZE   500

u32 red_buffer[BUFFER_SIZE];
u32 ir_buffer[BUFFER_SIZE];
int sample_rate = 100; // Hz



void max30102_write(u8 reg, u8 value) 
	{
    i2c_start();
    i2c_write(MAX30102_ADDR << 1); // Write address
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();
}

void max30102_init(void) {

     // Reset device

    max30102_write(0x09, 0x40);

    delay_ms(100);


    // FIFO pointers reset

    max30102_write(0x04, 0x00);

    max30102_write(0x05, 0x00);

    max30102_write(0x06, 0x00);


    // FIFO config

    max30102_write(0x08, 0x0F);


    // SpO2 mode

    max30102_write(0x09, 0x03);


    // SPO2 config

    // ADC range = 4096nA

    // Sample rate = 100Hz

    // Pulse width = 411us

    max30102_write(0x0A, 0x27);


    // LED pulse amplitude

    max30102_write(0x0C, 0x24); // RED LED

    max30102_write(0x0D, 0x24); // IR LED

				//max30102_write(0x0C, 0x3F);

			//max30102_write(0x0D, 0x3F);

    // Multi LED mode slots

    //max30102_write(0x11, 0x21);


    // Interrupt enable

    max30102_write(0x02, 0x40);

}
void read_fifo(u32 *red, u32 *ir) {
    u8 data[6];
    int i;

    i2c_start();
    i2c_write(MAX30102_ADDR << 1);
    i2c_write(0x07);
    i2c_restart();
    i2c_write((MAX30102_ADDR << 1) | 1);

    for(i = 0; i < 6; i++)
        data[i] = i2c_read(i < 5);

    i2c_stop();

    
    *red = (((u32)data[0] << 16) | ((u32)data[1] << 8) | (u32)data[2]) & 0x3FFFF;
	*ir  = (((u32)data[3] << 16) | ((u32)data[4] << 8) | (u32)data[5]) & 0x3FFFF;
}

int finger_detected(u32 *ir_data, int length)
{
    int i;
    u32 avg = 0;

    for(i = 0; i < length; i++)
    {
        avg += ir_data[i];
    }

    avg /= length;

    if(avg > 10000)
        return 1;
    else
        return 0;
}

unsigned int calculate_heart_rate(u32 *ir_data, int length)
{
    int i;
    long dc = 0;
    long ac_sum = 0;
    long threshold;
    int beats = 0;
    int last_beat = -1000;
    int interval_sum = 0;
    int interval;
    unsigned int bpm;

    for(i = 0; i < length; i++)
        dc += ir_data[i];

    dc /= length;

    for(i = 0; i < length; i++)
    {
        long v = (long)ir_data[i] - dc;
        if(v < 0)
            v = -v;
        ac_sum += v;
    }

    threshold = ac_sum / length;

    if(threshold < 300)
        threshold = 300;

    for(i = 2; i < length - 2; i++)
    {
        long prev = ((long)ir_data[i - 2] + ir_data[i - 1]) / 2 - dc;
        long curr = (long)ir_data[i] - dc;
        long next = ((long)ir_data[i + 1] + ir_data[i + 2]) / 2 - dc;

        if(curr < -threshold&&curr<prev&&curr < next &&(i - last_beat) > 65)
        {
    		if(last_beat > 0)
    		{
        		interval_sum += (i - last_beat);
        		beats++;
    		}
			 last_beat = i;
		}
    }

    if(beats < 2)
        return 0;

    interval = interval_sum / beats;
    bpm = (60 * sample_rate) / interval;

   // if(bpm < 50 || bpm > 140)
     //   return 0;

    return bpm;
}

/* ---------------- SIGNAL PROCESSING ---------------- */
/*unsigned int calculate_heart_rate(u32 *ir_data, int length) 
	{
		int i;
		int avg_interval;
    int peaks = 0, last_peak_index = -1, interval_sum = 0;
    for(i = 1; i < length-1; i++) {
       // if(ir_data[i] > ir_data[i-1] && ir_data[i] > ir_data[i+1]) 
			if(ir_data[i] > 50000 &&
   ir_data[i] > ir_data[i-1] &&
   ir_data[i] > ir_data[i+1])
			{
            peaks++;
            if(last_peak_index != -1) interval_sum += (i - last_peak_index);
            last_peak_index = i;
        }
    }
    if(peaks < 2) return 0;
    avg_interval = interval_sum / (peaks-1);
    return (60 * sample_rate) / avg_interval;
}*/

unsigned int calculate_spo2(u32 *red_data, u32 *ir_data, int length) 
	{
		long red_ac,ir_ac;
		int i;
		 float R;
		 unsigned int spo2;
	u32 red_min,red_max,ir_min,ir_max;
    long red_dc=0, ir_dc=0;
    for(i=0; i<length; i++) {
        red_dc += red_data[i];
        ir_dc  += ir_data[i];
    }
    red_dc /= length; ir_dc /= length;

     red_min=red_data[0], red_max=red_data[0];
     ir_min=ir_data[0], ir_max=ir_data[0];
    for(i=1; i<length; i++) {
        if(red_data[i]<red_min) red_min=red_data[i];
        if(red_data[i]>red_max) red_max=red_data[i];
        if(ir_data[i]<ir_min) ir_min=ir_data[i];
        if(ir_data[i]>ir_max) ir_max=ir_data[i];
    }
    red_ac = red_max - red_min;
    ir_ac  = ir_max - ir_min;
		
	if(red_dc == 0 || ir_dc == 0 || red_ac == 0 || ir_ac == 0)
    return 0;
		
    R = ((float)red_ac/red_dc) / ((float)ir_ac/ir_dc);
    spo2 = (unsigned int)(104 - 17*R);
    if(spo2>100) spo2=100;
    if(spo2<70)  
			return 0;
    return spo2;
}


