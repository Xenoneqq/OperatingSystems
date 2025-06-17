#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_PATIENTS_INSIDE 3
#define MAX_MEDKIT 6

int waiting_patients = 0;
int current_meds = MAX_MEDKIT;
int total_patients = 0;
int total_pharmacists = 0;
int patients_remaining;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t doctor_wakeup = PTHREAD_COND_INITIALIZER;
pthread_cond_t medkit_available = PTHREAD_COND_INITIALIZER;

int waiting_room[3];

void log_time() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);
}

void *patient_thread(void *arg) {
    int id = *(int *)arg;
    free(arg);
    while (1) {
        int delay = rand() % 4 + 2;
        log_time(); printf("Pacjent(%d): Ide do szpitala, bede za %d s\n", id, delay);
        sleep(delay);

        pthread_mutex_lock(&mutex);

        if (waiting_patients >= MAX_PATIENTS_INSIDE) {
            int walk = rand() % 4 + 2;
            log_time(); printf("Pacjent(%d): za duzo pacjentow, wracam pozniej za %d s\n", id, walk);
            pthread_mutex_unlock(&mutex);
            sleep(walk);
            continue;
        }

        waiting_room[waiting_patients] = id;
        waiting_patients++;
        log_time(); printf("Pacjent(%d): czeka %d pacjentow na lekarza\n", id, waiting_patients);

        if (waiting_patients == 3 && current_meds >= 3) {
            log_time(); printf("Pacjent(%d): budze lekarza\n", id);
            pthread_cond_signal(&doctor_wakeup);
        }

        pthread_mutex_unlock(&mutex);
        break;
    }
    return NULL;
}

void *pharmacist_thread(void *arg) {
    int id = *(int *)arg;
    free(arg);

    int delay = rand() % 11 + 5;
    log_time(); printf("Farmaceuta(%d): ide do szpitala, bede za %d s\n", id, delay);
    sleep(delay);

    pthread_mutex_lock(&mutex);

    while (current_meds >= 3) {
        log_time(); printf("Farmaceuta(%d): czekam na oproznienie apteczki\n", id);
        pthread_cond_wait(&medkit_available, &mutex);
    }

    log_time(); printf("Farmaceuta(%d): budze lekarza\n", id);
    pthread_cond_signal(&doctor_wakeup);

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *doctor_thread(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&mutex);

        while (!((waiting_patients >= 3 && current_meds >= 3) || (current_meds < 3))) {
            pthread_cond_wait(&doctor_wakeup, &mutex);
        }

        log_time(); printf("Lekarz: budze sie\n");

        if (waiting_patients >= 3 && current_meds >= 3) {
            int ids[3];
            for (int i = 0; i < 3; i++) {
                ids[i] = waiting_room[i];
            }
            waiting_patients -= 3;
            current_meds -= 3;
            log_time(); printf("Lekarz: konsultuje pacjentow %d, %d, %d\n", ids[0], ids[1], ids[2]);
            pthread_mutex_unlock(&mutex);

            sleep(rand() % 3 + 2); // 2-4s konsultacja

            for (int i = 0; i < 3; i++) {
                log_time(); printf("Pacjent(%d): koncze wizyte\n", ids[i]);
            }
            patients_remaining -= 3;
            pthread_cond_signal(&medkit_available);
        } else if (current_meds <= 3) {
            log_time(); printf("Lekarz: przyjmuje dostawe lekow\n");
            sleep(rand() % 3 + 1); // 1–3s dostawa
            current_meds += 3;
            log_time(); printf("Lekarz: apteczka uzupelniona do %d\n", current_meds);
        }

        log_time(); printf("Lekarz: zasypiam\n");
        pthread_mutex_unlock(&mutex);

        if (patients_remaining <= 0) break;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uzycie: %s <liczba_pacjentow> <liczba_farmaceutow>\n", argv[0]);
        return 1;
    }

    total_patients = atoi(argv[1]);
    total_pharmacists = atoi(argv[2]);
    patients_remaining = total_patients;

    pthread_t doctor;
    pthread_t patients[total_patients];
    pthread_t pharmacists[total_pharmacists];

    pthread_create(&doctor, NULL, doctor_thread, NULL);

    for (int i = 0; i < total_patients; i++) {
        int *id = malloc(sizeof(int)); *id = i + 1;
        pthread_create(&patients[i], NULL, patient_thread, id);
    }

    for (int i = 0; i < total_pharmacists; i++) {
        int *id = malloc(sizeof(int)); *id = i + 1;
        pthread_create(&pharmacists[i], NULL, pharmacist_thread, id);
    }

    for (int i = 0; i < total_patients; i++) {
        pthread_join(patients[i], NULL);
    }

    pthread_join(doctor, NULL);

    return 0;
}
