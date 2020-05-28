/*
 * classify.ino
 *
 * EE16B Spring 2016
 * Emily Naviasky & Nathaniel Mailoa
 *
 * EE 16B Fall 2017
 * Andrew Blatner
 *
 */

#define MIC_INPUT                   P6_0

#define SIZE                        2752
#define SIZE_AFTER_FILTER           172
#define ADC_TIMER_MS                0.35

/*---------------------------*/
/*      CODE BLOCK PCA1      */
/*---------------------------*/
#define SNIPPET_SIZE                  80
#define PRELENGTH                     5
#define THRESHOLD                     0.65

// Enveloping and K-means constants
//#define SNIPPET_SIZE                40
//#define PRELENGTH                   5
//#define THRESHOLD                   0.5

#define KMEANS_THRESHOLD            0.08
#define LOUDNESS_THRESHOLD          300

/*---------------------------*/
/*      CODE BLOCK PCA2      */
/*---------------------------*/
float pca_vec1[80] = {-0.000430843149559, -0.00153268756335, -0.016251935851, -0.0237154980221, 0.00652367701314, 0.0351004228002, 0.046319387949, 0.0458241899738, 0.0398743184025, 0.058480503278, 0.0741771200465, 0.0526442250129, 0.0802765812371, 0.111654122668, 0.179773136315, 0.227945512131, 0.260718228068, 0.290105471866, 0.301906017591, 0.299026012313, 0.278610359063, 0.25528702528, 0.2274431333, 0.213798842319, 0.171765163841, 0.110266633778, 0.0550269684256, -0.00355000347488, -0.0651830275727, -0.0921400045716, -0.107554904446, -0.103876569367, -0.0927102644452, -0.0876994098773, -0.076698360614, -0.0644151476103, -0.0611603976662, -0.0593535121216, -0.0576370761592, -0.0462231984364, -0.0375126927322, -0.0340829412316, -0.0359126396104, -0.0420464791531, -0.0500649517329, -0.0514998463913, -0.0659635315158, -0.0693181164407, -0.0753623743478, -0.0940391469635, -0.0981799167619, -0.108749863935, -0.10485253657, -0.129022239952, -0.112300247607, -0.1155942508, -0.108527129354, -0.0821624780426, -0.0677360266864, -0.0534983410198, -0.0429525121641, -0.0375108534894, -0.0338718273771, -0.02782850679, -0.0165520604837, -0.0205421747026, -0.0221064529757, -0.0283942104241, -0.0318749829851, -0.0369548583244, -0.0422396545981, -0.0578672598196, -0.0509212556855, -0.058598843179, -0.068589322855, -0.0661324983129, -0.0752832236825, -0.0700252005663, -0.073322388093, -0.0644203743678};
float pca_vec2[80] = {0.00499765894988, 0.0289062024959, 0.0641199593192, 0.0915213346633, 0.107993655115, 0.157412594786, 0.164340538006, 0.206089237162, 0.202473752626, 0.249727059265, 0.242449098069, 0.248400770582, 0.18670021219, 0.104931007894, -0.0160284322065, -0.0746688653559, -0.0655425429245, -0.0498641314318, -0.0429895006438, -0.049547294897, -0.0711815358859, -0.0921788504503, -0.0953228480952, -0.0771969265748, -0.0706937584758, -0.0622600919611, -0.0577390644383, -0.0752840283318, -0.135356045569, -0.173254154626, -0.180679583408, -0.165256568344, -0.155306028254, -0.152484446858, -0.130837267279, -0.108101273387, -0.0936136899463, -0.0803412116719, -0.04771715325, -0.0342515809048, -0.00563610673138, 0.00661514611306, 0.0457741976798, 0.0820862044271, 0.0859411801918, 0.0964208077496, 0.126020551222, 0.13353664782, 0.140093746322, 0.126242179137, 0.117545575641, 0.101996214963, 0.0615152016146, 0.0806886772321, 0.0351140418789, 0.0254794942886, 0.0450276316423, 0.0677176854051, 0.058214408057, 0.0787636032143, 0.0781030413237, 0.0582334443491, 0.0493985024394, 0.0287923390772, 0.0152856284404, 0.0046153125177, -0.0107239043126, -0.0217379627627, -0.0416585069798, -0.0559683385208, -0.0805590306379, -0.108703577287, -0.110911409799, -0.1317662814, -0.148597755312, -0.147487597824, -0.160727205301, -0.146980144635, -0.145554329879, -0.134575517317};
float pca_vec3[80] = {0.0797750191528, 0.120699308325, 0.119968393146, 0.138202187468, 0.0964941350435, 0.106384662754, 0.110390380377, 0.0918171767044, 0.0827064050121, 0.158470755298, 0.129496008027, 0.141769395162, 0.125434559248, 0.158740671093, 0.207361298992, 0.144513711447, 0.0975549009324, -0.000176434547252, -0.0219632835279, -0.0833497579886, -0.132501685985, -0.15938389213, -0.178629935306, -0.156622507652, -0.160112006581, -0.142860943665, -0.119037636931, -0.0761953288266, -0.0243535224449, 0.000347821162205, 0.0639623685812, 0.146100440148, 0.154626348556, 0.129679287993, 0.155545313933, 0.159009542551, 0.160481193177, 0.174869003292, 0.169017934582, 0.1463735518, 0.118013862686, 0.0961743531741, 0.0777726724349, 0.0352801188038, -0.0151966888655, -0.0648379508433, -0.114335158879, -0.119681514543, -0.141243568426, -0.160920444206, -0.151282679773, -0.16460815973, -0.12744007298, -0.155352021718, -0.105338124912, -0.096848829888, -0.0909154727826, -0.0903929317836, -0.0788496132455, -0.0919858387255, -0.094025786793, -0.101834316027, -0.0989398385326, -0.0879460270451, -0.0823677597184, -0.0834108670066, -0.0763295095602, -0.0722015049716, -0.0689599970527, -0.0580714468849, -0.0417848771501, -0.0357093407971, -0.0254999000763, -0.0116450857508, -0.00738553938339, 0.00595496395282, -0.00457672862507, 0.00906430649846, 0.0269919945473, 0.036060486208};
float projected_mean_vec[3] = {0.0343971541137, 0.0451764956238, 0.0416660458979};
float centroid1[3] = {0.0834937288625, -0.0114823737323, -0.0153620808679};
float centroid2[3] = {-0.0414405141673, -0.0525951375641, -0.00309076643407};
float centroid3[3] = {-0.0311943445418, 0.0509979067963, -0.030908678595};
float centroid4[3] = {0.0030227124055, 0.0176476893703, 0.0479980465154};

//float pca_vec1[SNIPPET_SIZE] = ;
//float pca_vec2[SNIPPET_SIZE] = ;
//float projected_mean_vec[2] = ;
//float centroid1[2] = ;
//float centroid2[2] = ;
//float centroid3[2] = ;
//float centroid4[2] = ;
//float* centroids[4] = {
//  (float *) &centroid1, (float *) &centroid2,
//  (float *) &centroid3, (float *) &centroid4
//};

/*---------------------------*/
/*---------------------------*/
/*---------------------------*/

float result[SNIPPET_SIZE] = {0};
float proj1 = 0;
float proj2 = 0;
float proj3 = 0;

// Data array and index pointer
int re[SIZE] = {0};
volatile int re_pointer = 0;

/*---------------------------*/
/*       Norm functions      */
/*---------------------------*/

// Compute the L2 norm of (dim1, dim2) and centroid
// input: dim1: 1st dimension coordinate
//        dim2: 2nd dimension coordinate
//        centroid: size-2 array containing centroid coordinates
// output: L2 norm (Euclidean distance) between point and centroid
float l2_norm(float dim1, float dim2, float* centroid) {
  return sqrt(pow(dim1-centroid[0],2) + pow(dim2-centroid[1],2));
}

// Compute the L2 norm of (dim1, dim2, dim3) and centroid
// input: dim1: 1st dimension coordinate
//        dim2: 2nd dimension coordinate
//        dim3: 3rd dimension coordinate
//        centroid: size-3 array containing centroid coordinates
// output: L2 norm (Euclidean distance) between point and centroid
float l2_norm3(float dim1, float dim2, float dim3, float* centroid) {
  return sqrt(pow(dim1-centroid[0],2) + pow(dim2-centroid[1],2) + pow(dim3-centroid[2],2));
}

void setup(void) {
  Serial.begin(38400);

  pinMode(MIC_INPUT, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  re_pointer = 0;
  reset_blinker();
  setTimer();
}

void loop(void) {
  if (re_pointer == SIZE) {
    digitalWrite(RED_LED, LOW);

    // Apply enveloping function and get snippet with speech.
    // Do classification only if loud enough.
    if (envelope(re, result)) {

      // Reset projection result variables declared above
      proj1 = 0;
      proj2 = 0;
      proj3 = 0;

      /*---------------------------*/
      /*      CODE BLOCK PCA3      */
      /*---------------------------*/

      // Project 'result' onto the principal components
      // Hint: 'result' is an array
      // Hint: do this entire operation in 1 loop by replacing the '...'
      // YOUR CODE HERE
      for (int i = 0; i < SNIPPET_SIZE; i++) {
          proj1 += result[i] * pca_vec1[i];
          proj2 += result[i] * pca_vec2[i];
          proj3 += result[i] * pca_vec3[i];
      }

      // Demean the projection
      proj1 -= projected_mean_vec[0];
      proj2 -= projected_mean_vec[1];
      proj3 -= projected_mean_vec[2];

      // Classification
      // Use the function 'l2_norm' defined above
      // ith centroid: 'centroids[i]'
      float best_dist = 999999;
      int best_index = -1;
      // YOUR CODE HERE
      float dist[] = {l2_norm3(proj1,proj2,proj3,centroid1), l2_norm3(proj1,proj2,proj3,centroid2), l2_norm3(proj1,proj2,proj3,centroid3), l2_norm3(proj1,proj2,proj3,centroid4)};
      for (int i = 0; i < 4; i++) {
        if (dist[i] < best_dist) {
          best_dist = dist[i];
          best_index = i;
        }
      }     


      // Compare 'best_dist' against the 'KMEANS_THRESHOLD' and print the result
      // If 'best_dist' is less than the 'KMEANS_THRESHOLD', the recording is a word
      // Otherwise, the recording is noise
      // YOUR CODE HERE
      
        Serial.println(dist[0]);
        Serial.println(dist[1]);
        Serial.println(dist[2]);
        Serial.println(dist[3]);
      if (best_dist < KMEANS_THRESHOLD) {
        Serial.println(best_index);
      } else {
        Serial.println("Noise");
      }


      /*---------------------------*/
      /*---------------------------*/
      /*---------------------------*/
    }
    else {
      Serial.println("Below LOUDNESS_THRESHOLD.");
    }


    delay(2000);
    re_pointer = 0;
  }
}

// Enveloping function with thresholding and normalizing,
// returns snippet of interest (containing speech)
bool envelope(int* data, float* data_out) {
  int32_t avg = 0;
  float maximum = 0;
  int32_t total = 0;
  int block;

  // Apply enveloping filter while finding maximum value
  for (block = 0; block < SIZE_AFTER_FILTER; block++) {
    avg = 0;
    for (int i = 0; i < 16; i++) {
      avg += data[i+block*16];
    }
    avg = avg >> 4;
    data[block] = abs(data[block*16] - avg);
    for (int i = 1; i < 16; i++) {
      data[block] += abs(data[i+block*16] - avg);
    }
    if (data[block] > maximum) {
      maximum = data[block];
    }
  }

  // If not loud enough, return false
  if (maximum < LOUDNESS_THRESHOLD) {
    return false;
  }

  // Determine threshold
  float thres = THRESHOLD * maximum;

  // Figure out when interesting snippet starts and write to data_out
  block = PRELENGTH;
  while (data[block++] < thres);
  if (block > SIZE_AFTER_FILTER - SNIPPET_SIZE) {
    block = SIZE_AFTER_FILTER - SNIPPET_SIZE;
  }
  for (int i = 0; i < SNIPPET_SIZE; i++) {
    data_out[i] = data[block-PRELENGTH+i];
    total += data_out[i];
  }

  // Normalize data_out
  for (int i = 0; i < SNIPPET_SIZE; i++) {
    data_out[i] = data_out[i] / total;
  }

  return true;
}

/*---------------------------*/
/*     Helper functions      */
/*---------------------------*/

void reset_blinker(void) {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  delay(100);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  delay(100);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(100);
  digitalWrite(GREEN_LED, LOW);
}

/*---------------------------*/
/*    Interrupt functions    */
/*---------------------------*/

// ISR for timestep
#pragma vector=TIMER2_A0_VECTOR    // Timer A ISR
__interrupt void Timer2_A0_ISR(void) {
  if (re_pointer < SIZE) {
    digitalWrite(RED_LED, HIGH);
    re[re_pointer] = (analogRead(MIC_INPUT) >> 4) - 128;
    re_pointer += 1;
  }
}

// Set timer for timestep; use A2 since A0 & A1 are used by PWM
void setTimer(void) {
  // Set the timer based on 25MHz clock
  TA2CCR0 = (unsigned int) (25000*ADC_TIMER_MS);
  TA2CCTL0 = CCIE;
  __bis_SR_register(GIE);
  TA2CTL = TASSEL_2 + MC_1 + TACLR + ID_0;
}
