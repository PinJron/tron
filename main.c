#include "controller.h"

// In reciever() recv enemy's key
// In main() our key press to enemy

in_addr_t get_local_ip() {
    FILE *f;
    char line[100], *p, *c;

    f = fopen("/proc/net/route", "r");

    while (fgets(line, 100, f))
    {
        p = strtok(line, " \t");
        c = strtok(NULL, " \t");

        if (p != NULL && c != NULL)
        {
            // 0064A8C0 00000000
            if (strcmp(c, "007AA8C0") == 0)
            {
                break;
            }
        }
    }

    int fm = AF_INET;
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        family = ifa->ifa_addr->sa_family;

        if (strcmp(ifa->ifa_name, p) == 0)
        {
            if (family == fm)
            {
                s = getnameinfo(ifa->ifa_addr,
                                (family == AF_INET) ? sizeof(struct sockaddr_in)
                                                    : sizeof(struct sockaddr_in6),
                                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

                if (s != 0)
                {
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    freeifaddrs(ifaddr);

    return inet_addr( host );
}

// Для Димы:
//      Две процедуры отвечают за "парсинг" нажатия.
//      Они вроде как одинаковые, так что можно одну оставить
void *manage_red_tron(void *thread_data) {
    struct tron *data = (struct tron *)thread_data;

    if (*data->key != 'a' && *data->key != 's' && *data->key != 'w' &&
        *data->key != 'd' && *data->key != 'q')
        *data->key = *data->key_prev;
    switch (*data->key)
    {
        case 's':
        {
            if (*data->key_prev == 'w')
            {
                *data->key = *data->key_prev;
                break;
            }
            (*data->tail_y)++;
            for (int i = 0; i < 40; i++)
            {
                data->tron->y[i] = *data->tail_y + 1 + i % 8;
                data->tron->x[i] = *data->tail_x + 2 - i / 8;
            }
            break;
        }
        case 'd':
        {
            if (*data->key_prev == 'a')
            {
                *data->key = *data->key_prev;
                break;
            }
            (*data->tail_x)++;
            for (int i = 0; i < 40; i++)
            {
                data->tron->y[i] = *data->tail_y - 2 + i / 8;
                data->tron->x[i] = *data->tail_x + 1 + i % 8;
            }
            break;
        }
        case 'w':
        {
            if (*data->key_prev == 's')
            {
                *data->key = *data->key_prev;
                break;
            }
            (*data->tail_y)--;
            for (int i = 0; i < 40; i++)
            {
                data->tron->y[i] = *data->tail_y - 1 - i % 8;
                data->tron->x[i] = *data->tail_x - 2 + i / 8;
            }
            break;
        }
        case 'a':
        {
            if (*data->key_prev == 'd')
            {
                *data->key = *data->key_prev;
                break;
            }
            (*data->tail_x)--;
            for (int i = 0; i < 40; i++)
            {
                data->tron->y[i] = *data->tail_y + 2 - i / 8;
                data->tron->x[i] = *data->tail_x - 1 - i % 8;
            }
            break;
        }
        default:
            break;
    }
    *data->key_prev = *data->key;
}

void *manage_blue_tron(void *thread_data) {
    struct tron *data = (struct tron *)thread_data;
    if (*data->key != 'a' && *data->key != 's' && *data->key != 'w' &&
        *data->key != 'd' && *data->key != 'q')
        *data->key = *data->key_prev;
    switch (*data->key)
    {
        case 's':
        {
            if (*data->key_prev == 'w')
            {
                *data->key = *data->key_prev;
                break;
            }
            (*data->tail_y)++;
            for (int i = 0; i < 40; i++)
            {
                data->tron->y[i] = *data->tail_y + 1 + i % 8;
                data->tron->x[i] = *data->tail_x + 2 - i / 8;
            }
            break;
        }
        case 'd':
        {
            if (*data->key_prev == 'a')
            {
                *data->key = *data->key_prev;
                break;
            }
            (*data->tail_x)++;
            for (int i = 0; i < 40; i++)
            {
                data->tron->y[i] = *data->tail_y - 2 + i / 8;
                data->tron->x[i] = *data->tail_x + 1 + i % 8;
            }
            break;
        }
        case 'w':
        {
            if (*data->key_prev == 's')
            {
                *data->key = *data->key_prev;
                break;
            }
            (*data->tail_y)--;
            for (int i = 0; i < 40; i++)
            {
                data->tron->y[i] = *data->tail_y - 1 - i % 8;
                data->tron->x[i] = *data->tail_x - 2 + i / 8;
            }
            break;
        }
        case 'a':
        {
            if (*data->key_prev == 'd')
            {
                *data->key = *data->key_prev;
                break;
            }
            (*data->tail_x)--;
            for (int i = 0; i < 40; i++)
            {
                data->tron->y[i] = *data->tail_y + 2 - i / 8;
                data->tron->x[i] = *data->tail_x - 1 - i % 8;
            }
            break;
        }
        default:
            break;
    }
    *data->key_prev = *data->key;
}

// Для Димы:
//      Процедура для "серверного" потока
void reciever(struct server_data *data) {
    while(data->work_flag)
    {
        socklen_t len = sizeof(data->net->enemy_addr);
        recvfrom(data->net->nd, data->enemy_tron->key, 1, 0,
                 (struct sockaddr*)&data->net->enemy_addr, &len);
        data->manage_enemy_tron(data->enemy_tron);
    }
}

// Для Димы:
//      "Серверный" поток
void *thread_server(void *thread_data) {
    struct server_data *data = (struct server_data *)thread_data;
    reciever(data);
}

// Для Димы:
//      Процедура для "клиентского" потока
int sender(struct server_data *data) {
    while(data->work_flag)
    {
        int ch = getch();
        if(ch != 'w' && ch != 'a' && ch != 's' && ch != 'd')
        {
            continue;
        }

        *data->my_tron->key = ch;
        sendto(data->net->nd, (const int*)&ch, 1, MSG_CONFIRM,
               (struct sockaddr *)&data->net->enemy_addr, sizeof(data->net->enemy_addr));
        data->manage_my_tron(data->my_tron);
    }

    return 0;
}

// Для Димы:
//      "Клиентский" поток
void *thread_client(void *thread_data) {
    struct server_data *data = (struct server_data *)thread_data;
    sender(data);
}

void print(int user_res_x, int user_res_y, struct fb_var_screeninfo *info, uint32_t *ptr,
           uint32_t red, uint32_t blue, uint32_t prpl, struct length *red_tron,
           struct length *blue_tron, unsigned int tail_x1, unsigned int tail_x2, unsigned int tail_y1, unsigned int tail_y2) {
    for (int i = 0; i < user_res_x; i++)
    {
        ptr[(info->yres - user_res_y) / 2 * info->xres_virtual + (info->xres - user_res_x) / 2 + i] = prpl;
        ptr[(info->yres + user_res_y) / 2 * info->xres_virtual + (info->xres - user_res_x) / 2 + i] = prpl;
    }
    for (int i = 0; i < user_res_y; i++)
    {
        ptr[(info->yres - user_res_y) / 2 * info->xres_virtual + (info->xres - user_res_x) / 2 + i * info->xres_virtual] = prpl;
        ptr[(info->yres - user_res_y) / 2 * info->xres_virtual + (info->xres + user_res_x) / 2 + i * info->xres_virtual] = prpl;
    }
    for (int i = 0; i < 40; i++)
    {
        ptr[red_tron->y[i] * info->xres_virtual + red_tron->x[i]] = red;
        ptr[blue_tron->y[i] * info->xres_virtual + blue_tron->x[i]] = blue;
    }
    ptr[tail_y1 * info->xres_virtual + tail_x1] = red;
    ptr[tail_y2 * info->xres_virtual + tail_x2] = blue;
}

int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <WIDTHTxHEIGHT> <ipaddr>\n", argv[0]);
        return -1;
    }

    // Get enemy ip address
    char enemy_ip_str[15];
    sscanf(argv[2], "%s", enemy_ip_str);
    in_addr_t enemy_ip = inet_addr(enemy_ip_str);
    // Get local ip address
    in_addr_t my_ip = get_local_ip();

    pthread_t my_thread, enemy_thread;

    uint8_t work_flag = 1;
    int pointwin1 = 0, pointwin2 = 0;
    int i = 0;
    int red_plr_key = 0, blue_plr_key = 0;
    int red_plr_key_prev = 0, blue_plr_key_prev = 0;

    // Init network stuff
    // --------
    struct network net;
    create_socket(enemy_ip, my_ip, 12345, &net);
    // Bind socket to local ip
    if (bind(net.nd, (const struct sockaddr *)&net.my_addr, sizeof(net.my_addr)) != 0)
    {
        perror("bind");
        return -1;
    }
    // --------

    int fb;
    struct fb_var_screeninfo info;
    size_t fb_size, map_size, page_size;
    uint32_t *ptr, clr_red, clr_blue, clr_prpl;
    int user_res_x, user_res_y;
    sscanf(argv[1], "%dx%d", &user_res_x, &user_res_y);

    clr_red = 0xff0000;
    clr_blue = 0x0000ff;
    clr_prpl = 0x9900ff;

    // Framebuffer
    // --------
    page_size = sysconf(_SC_PAGESIZE);
    if (0 > (fb = open("/dev/fb0", O_RDWR)))
    {
        perror("open");
        endwin();
        return __LINE__;
    }
    if ((-1) == ioctl(fb, FBIOGET_VSCREENINFO, &info))
    {
        perror("ioctl");
        close(fb);
        endwin();
        return __LINE__;
    }
    fb_size = sizeof(uint32_t) * info.xres_virtual * info.yres_virtual;
    map_size = (fb_size + (page_size - 1)) & (~(page_size - 1));
    ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if (MAP_FAILED == ptr)
    {
        perror("mmap");
        close(fb);
        endwin();
        return __LINE__;
    }
    // Check input resolution
    if (user_res_x > info.xres || user_res_y > info.yres)
    {
        fprintf(stderr, "User resolution too big!\n");
        close(fb);
        endwin();
        return __LINE__;
    }
    // --------

    // Init trons
    // --------
    // Для Димы:
    //      Схему отрисовки байков не менял.
    //      Только распихал данные по структурам.
    struct length red_tron, blue_tron;
    for (i = 0; i < 40; i++)
    {
        red_tron.y[i] = info.yres / 2 - user_res_y / 2 + 20 + i / 8;
        red_tron.x[i] = info.xres / 2 - user_res_x / 2 + 20 + i % 8;
    }
    for (i = 0; i < 40; i++)
    {
        blue_tron.y[i] = info.yres / 2 + user_res_y / 2 - 20 - i / 8;
        blue_tron.x[i] = info.xres / 2 + user_res_x / 2 - 20 - i % 8;
    }
    unsigned int tail_y1 = red_tron.y[16];
    unsigned int tail_x1 = red_tron.x[16];
    unsigned int tail_y2 = blue_tron.y[16];
    unsigned int tail_x2 = blue_tron.x[16];

    // Data for network threads
    // --------
    // Для Димы:
    //      Эта структура хранит все данные вместе.
    //      Ее передаем по всем потокам и процедурам.
    struct server_data server_data = {
            .net = &net,
            .work_flag = &work_flag,
    };
    server_data.my_tron = malloc(sizeof(struct tron));
    server_data.enemy_tron = malloc(sizeof(struct tron));
    pthread_mutex_init(&server_data.my_tron->mutex, NULL);
    pthread_mutex_init(&server_data.enemy_tron->mutex, NULL);
    // --------

    // FIXME: segfault here
    // Tron assignment
    // --------
    // Для Димы:
    //      У кого ip больше, тот красный
    if (my_ip > enemy_ip)
    {
        server_data.my_tron->tron = &red_tron;
        server_data.my_tron->key = &red_plr_key;
        server_data.my_tron->key_prev = &red_plr_key_prev;
        server_data.my_tron->tail_x = &tail_x1;
        server_data.my_tron->tail_y = &tail_y1;
        // Для Димы:
        //      Присваиваю адрес процедуры, которая "управляет красным байком"
        //      Она будет вызываться в соответсвующем потоке, как поле структуры.
        //
        //      Так как, вроде, в процедурах управления нет разницы,
        //      можно будет это убрать и просто вызывать процедуру.
        server_data.manage_my_tron = manage_red_tron;

        server_data.enemy_tron->tron = &blue_tron;
        server_data.enemy_tron->key = &blue_plr_key;
        server_data.enemy_tron->key_prev = &blue_plr_key_prev;
        server_data.enemy_tron->tail_x = &tail_x2;
        server_data.enemy_tron->tail_y = &tail_y2;
        server_data.manage_enemy_tron = manage_blue_tron;
    }
    else
    {
        server_data.my_tron->tron = &blue_tron;
        server_data.my_tron->key = &blue_plr_key;
        server_data.my_tron->key_prev = &blue_plr_key_prev;
        server_data.my_tron->tail_x = &tail_x2;
        server_data.my_tron->tail_y = &tail_y2;
        server_data.manage_my_tron = manage_blue_tron;

        server_data.enemy_tron->tron = &red_tron;
        server_data.enemy_tron->key = &red_plr_key;
        server_data.enemy_tron->key_prev = &red_plr_key_prev;
        server_data.enemy_tron->tail_x = &tail_x1;
        server_data.enemy_tron->tail_y = &tail_y1;
        server_data.manage_enemy_tron = manage_red_tron;
    }
    // --------

    // Curses init
    // --------
    initscr();
    noecho();
    // Для Димы:
    //      nodelay() делает getch() non-blocking процедурой,
    //      то есть она не ждет твоего нажатия, а присваивает ERR сразу.
    //      Не знаю нужно ли будет это в твоей программе, но у меня на этом построено взаимодействие.
    nodelay(stdscr, TRUE);
    keypad(stdscr, true);
    refresh();
    // --------

    // Wait for the enemy's response
    // --------
    // Для Димы:
    //      Нажимаем любую клавишу и ждем пока противник отправит свою,
    //      тем самым подтверждаем начало игры
    int key;
    printw("Enter a key: ");
    do {
        key = getch();
    } while ( key == ERR );
    sendto(server_data.net->nd, (const int *) &key, 1, MSG_CONFIRM,
           (struct sockaddr *) &server_data.net->enemy_addr, sizeof(server_data.net->enemy_addr));
    // Для Димы:
    //      Закомментил, чтобы тестить без второй виртуалки.

//    socklen_t len = sizeof(server_data.net->enemy_addr);
//    recvfrom(server_data.net->nd, &key, 1, 0,
//             (struct sockaddr*)&server_data.net->enemy_addr, &len);
    clear();
    // --------

    pthread_create(&enemy_thread, NULL, thread_server, &server_data);
    pthread_create(&my_thread, NULL, thread_client, &server_data);

    print(user_res_x, user_res_y, &info, ptr, clr_red, clr_blue, clr_prpl,
          &red_tron, &blue_tron, tail_x1, tail_x2, tail_y1, tail_y2);


    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Для Димы:
    //      По сути осталось разобраться с этим game loop'ом,
    //      а именно с отрисовкой и ее синхронизацией,
    //      проверкой столкновений и ожиданием.
    //
    //      Текущая проблема: байки не ездят сами по себе
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    // Game loop
    while (work_flag)
    {
        // Для Димы:
        //      По поводу мьютексов: здесь, скорее всего,
        //      нам нужно будет ожидать получения нажатий из потоков.
        //      Поэтому процедуру отрисовки "ставим на паузу".

//        pthread_mutex_lock(&server_data.my_tron->mutex);
//        pthread_mutex_lock(&server_data.enemy_tron->mutex);

        print(user_res_x, user_res_y, &info, ptr, clr_red, clr_blue, clr_prpl,
              &red_tron, &blue_tron, tail_x1, tail_x2, tail_y1, tail_y2);

        if (red_plr_key == 0 && blue_plr_key == 0) continue;
        usleep(62500);

        for (i = 0; i < 40; i++)
        {
            ptr[red_tron.y[i] * info.xres_virtual + red_tron.x[i]] = 0x00000000;
            ptr[blue_tron.y[i] * info.xres_virtual + blue_tron.x[i]] = 0x00000000;
        }
//        pthread_mutex_unlock(&server_data.my_tron->mutex);
//        pthread_mutex_unlock(&server_data.enemy_tron->mutex);

        for (i = 0; i < 40; i++)
        {
            if (ptr[red_tron.y[i] * info.xres_virtual + red_tron.x[i]] == clr_red ||
                ptr[red_tron.y[i] * info.xres_virtual + red_tron.x[i]] == clr_blue ||
                ptr[red_tron.y[i] * info.xres_virtual + red_tron.x[i]] == clr_prpl ||
                (red_tron.y[i] == blue_tron.y[i] && red_tron.x[i] == blue_tron.x[i]))
                pointwin1 = 1;
            if (red_plr_key == 0)
                pointwin1 = 0;
        }

        for (i = 0; i < 40; i++)
        {
            if (ptr[blue_tron.y[i] * info.xres_virtual + blue_tron.x[i]] == clr_red ||
                ptr[blue_tron.y[i] * info.xres_virtual + blue_tron.x[i]] == clr_blue ||
                ptr[blue_tron.y[i] * info.xres_virtual + blue_tron.x[i]] == clr_prpl ||
                (red_tron.y[i] == blue_tron.y[i] && red_tron.x[i] == blue_tron.x[i]))
                pointwin2 = 1;
            if (blue_plr_key == 0)
                pointwin2 = 0;
        }

        if (pointwin1 == 1 && pointwin2 == 1)
        {
            work_flag = 0;
            printw("Draw\n");
            break;
        }

        if (pointwin1 == 1 && pointwin2 == 0)
        {
            work_flag = 0;
            printw("Win Player 2\n");
            break;
        }

        if (pointwin1 == 0 && pointwin2 == 1)
        {
            work_flag = 0;
            printw("Win Player 1\n");
            break;
        }

        usleep(62500);
    }

    munmap(ptr, map_size);
    close(fb);

    free(server_data.my_tron);
    free(server_data.enemy_tron);

    pthread_join(enemy_thread, NULL);
    pthread_join(my_thread, NULL);

    endwin();
}
