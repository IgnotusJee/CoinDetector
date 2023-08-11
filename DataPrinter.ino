#include <Arduino.h>
#include <ctype.h>
#include <math.h>
#include <Printable.h>

#define AVG_TIME 50

unsigned amp_in_gpio = 26, diff_in_gpio = 25, freq_in_gpio = 33, CountingValue;
// unsigned amp_in_gpio = 32, diff_in_gpio = 33, freq_in_gpio = 25, CountingValue;
float TimeInterval_Calc, TimeInterval_Print;

struct MonitorData {
  float val, avg, avg2, del, del2;
  MonitorData(float val=0, float avg=0, float avg2=0, float del=0, float del2=0) :val(val), avg(avg), avg2(avg2), del(del), del2(del2) {}
} amp, diff, freq;

float GetAmp() {
  return analogRead(amp_in_gpio);
}

float GetDiff() {
  return analogRead(diff_in_gpio);
}

float GetFreq() {
  static float ans;
  ans = (float) CountingValue / TimeInterval_Calc / 2;
  CountingValue = 0;
  return ans;
}

typedef float (*func)(void);
struct Task {
  MonitorData* data_ptr;
  func func_ptr;
  Task(MonitorData* data_ptr, func func_ptr) : data_ptr(data_ptr), func_ptr(func_ptr) {}
};

Task *task_amp, *task_diff, *task_freq;

// 每隔TimeInterval秒计算
void DataDealer(void *ptParam) {
  float queue[AVG_TIME], queue2[AVG_TIME];
  float &val = ((Task*)ptParam)->data_ptr->val, &avg = ((Task*)ptParam)->data_ptr->avg, &avg2 = ((Task*)ptParam)->data_ptr->avg2, &del = ((Task*)ptParam)->data_ptr->del, &del2 = ((Task*)ptParam)->data_ptr->del2;
  unsigned end = 0, i;
  for(i = 0; i < AVG_TIME; i++) {
    queue[i] = queue2[i] = 0;
  }
  while(1) {
      val = ((Task*)ptParam)->func_ptr();

      queue[end] = val;

      avg = 0;
      for(i = 0; i < AVG_TIME; i++) {
          avg += queue[i];
      }
      avg /= 1.0 * AVG_TIME;

      del = 0;
      for(i = 0; i < AVG_TIME; i++) {
          del += (queue[i] - avg) * (queue[i] - avg);
      }
      del /= 1.0 * AVG_TIME;

      queue2[end] = avg;

      avg2 = 0;
      for(i = 0; i < AVG_TIME; i++) {
          avg2 += queue2[i];
      }
      avg2 /= 1.0 * AVG_TIME;

      del2 = 0;
      for(i = 0; i < AVG_TIME; i++) {
          del2 += (queue2[i] - avg2) * (queue2[i] - avg2);
      }
      del2 /= 1.0 * AVG_TIME;

      end = (end + 1) % AVG_TIME;
      vTaskDelay((unsigned int)(1000 * TimeInterval_Calc / portTICK_PERIOD_MS));//转换成 mS
  }
}

void PrintData() {
  // Serial.print("val_amp:");
  // Serial.print(amp.val);
  // Serial.print(" val_diff:");
  // Serial.print(diff.val);
  // Serial.print(" val_freq:");
  // Serial.print(freq.val);
  Serial.print(" avg_amp:");
  Serial.print(amp.avg);
  Serial.print(" avg_diff:");
  Serial.print(diff.avg);
  Serial.print(" avg_freq:");
  Serial.print(freq.avg);
  // Serial.print(" del_amp:");
  // Serial.print(amp.del);
  // Serial.print(" del_diff:");
  // Serial.print(diff.del);
  // Serial.print(" del_freq:");
  // Serial.print(freq.del);
  Serial.print(" del2_amp:");
  Serial.print(amp.del2);
  Serial.print(" del2_diff:");
  Serial.print(diff.del2);
  Serial.print(" del2_freq:");
  Serial.print(freq.del2);
  Serial.println("");
}

// 输出当前队列的计算数据
void GetCurData(void *ptParam) {
  while(1) {
    PrintData();
    vTaskDelay((unsigned int)(1000* TimeInterval_Print));
  }
}

struct StoredData :public Printable{
  float amp, diff, freq;
  StoredData(float amp=0, float diff=0, float freq=0) : amp(amp), diff(diff), freq(freq) {}
  size_t printTo(Print& p) const override {
    size_t n = 0;
    n += p.print(amp);
    n += p.print(' ');
    n += p.print(diff);
    n += p.print(' ');
    n += p.print(freq);
    return n;
  }
  bool operator < (const StoredData &x) const {
    return this->amp < x.amp && this->diff < x.diff && this->freq < x.freq;
  }
  StoredData operator - (const StoredData &x) const {
    return StoredData(this->amp - x.amp, this->diff - x.diff, this->freq - x.freq);
  }
};

void command(void *ptParam) {
  // 命令交互系统，用于和PC通讯
  StoredData base, now_del, max_del(1, 1, 1), max_err(0.02, 0.02, 0.02);
  unsigned i;
  char com_char;

#define GET_SP_DATA(obj) StoredData(amp.obj, diff.obj, freq.obj)

  while(1) {
    com_char = Serial.read();
    if(com_char == 0xff || isblank(com_char)) continue;
    now_del = GET_SP_DATA(del2);
    if(com_char == 's') { // 设置基础数据
      if (now_del < max_del) {
        base = GET_SP_DATA(avg);
        Serial.print("Set base to ");
        Serial.println(base);
      }
      else {
        Serial.println("Data not stable yet. Please try again later.");
      }
    }
    else if(com_char == 'p') { // 打印当前数据
      Serial.println(GET_SP_DATA(avg));
      Serial.println(GET_SP_DATA(del2));
    }
    else if (com_char == 't') {
      Serial.println(GET_SP_DATA(del2));
    }
    else if(com_char == 'd') { // 打印差值
      if (now_del < max_del) {
        Serial.println(GET_SP_DATA(avg) - base);
      }
      else {
        Serial.println("Data not stable yet. Please try again later.");
      }
    }
  }

  // while (1) {
  //   com_char = Serial.read();
  //   if (com_char == 0xff || isblank(com_char)) continue;
  //   if (com_char == '0') {
  //     if (!(now_del < max_del)) {
  //       Serial.println("Data not stable yet. Please try again later.");
  //     } else {
  //       base = 
  //       Serial.print("Set base data to ");
  //       Serial.println(base);
  //     }
  //   } else if (com_char > '0' && com_char <= '0' + COIN_TYPE_NUM) {
  //     if (!(now_del < max_del)) {
  //       Serial.println("Data not stable yet. Please try again later.");
  //     } else {
  //       now = com_char - '1';
  //       fea[now] = GET_SP_DATA(avg);
  //       Serial.print("Set num ");
  //       Serial.print(now + 1);
  //       Serial.print(" data to ");
  //       Serial.println(fea[now]);
  //     }
  //   } else if (com_char == 'p') {
  //     PrintData();
  //   } else if (com_char == 'd') {
  //     if (!(now_del < max_del)) {
  //       Serial.println("Data not stable yet. Please try again later.");
  //     } else {
  //       now = -1;
  //       now = avg;
  //       for (i = 0; i < COIN_TYPE_NUM; i++) {
  //         if (now == -1 || abs(now_freq - fea_freq[i]) < abs(now_freq - fea_freq[now])) {
  //           now = i;
  //         }
  //       }
  //       if (now == -1 || abs(now_freq - fea_freq[now]) / base_freq > MAX_ERR) {
  //         Serial.println("No coin matched.");
  //       } else {
  //         Serial.print("Detected coin number ");
  //         Serial.print(now + 1);
  //         Serial.print(" , whose freq is ");
  //         Serial.println(fea_freq[now]);
  //       }
  //     }
  //   }
  //   // else {
  //   //   Serial.println("Invalid command");
  //   // }
  // }
}

void PulseCounting(void) {
  CountingValue++;
}

void setup() {
  Serial.begin(115200);

  TimeInterval_Calc = 0.05;
  TimeInterval_Print = 0.05;

  CountingValue = 0;
  
  pinMode(freq_in_gpio, INPUT); // 设置引脚状态

  attachInterrupt(freq_in_gpio, PulseCounting, CHANGE);

  task_amp = new Task(&amp, GetAmp);
  xTaskCreatePinnedToCore(DataDealer, "Get amp",  4096, task_amp, 1, NULL, 1);

  task_diff = new Task(&diff, GetDiff);
  xTaskCreatePinnedToCore(DataDealer, "Get diff",  4096, task_diff, 1, NULL, 1);

  task_freq = new Task(&freq, GetFreq);
  xTaskCreatePinnedToCore(DataDealer, "Get freq",  4096, task_freq, 1, NULL, 1);

  xTaskCreatePinnedToCore(command, "Command sys", 2048, NULL, 1, NULL, 1);

  // xTaskCreatePinnedToCore(GetCurData, "PaintData",  2048, NULL, 1, NULL, 1);
}

void loop() {

}

