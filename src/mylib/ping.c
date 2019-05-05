#include "ping.h"

static int send_order = 1;
static int recv_order = 0;
char send_buff[SEND_BUFFER_SIZE];
char recv_buff[RECV_BUFFER_SIZE];
struct hostent *host = NULL; //host entry, eg: host->h_name
int sock_icmp;				 //icmp socket
char *ip_str = NULL;
struct timeval t_send, t_recv;
double max = 0, min = 0, avg = 0, mdev = 0;

u_int16_t get_cksum(struct icmp *icmp_ptr)
{
	u_int16_t *data = (u_int16_t *)icmp_ptr;
	int len = ICMP_LEN;
	u_int32_t sum = 0;

	while (len > 1)
	{
		sum += *data++;
		len -= 2;
	}
	if (1 == len)
	{
		u_int16_t tmp = *data;
		tmp &= 0xff00;
		sum += tmp;
	}

	//check sum
	while (sum >> 16)
		sum = (sum >> 16) + (sum & 0x0000ffff);
	sum = ~sum;
	return sum;
}
void set_icmp(u_int16_t seq)
{
	struct icmp *picmp = NULL;
	struct timeval *ptime = NULL;

	picmp = (struct icmp *)send_buff;

	picmp->icmp_type = ICMP_ECHO; //request
	picmp->icmp_code = 0;

	picmp->icmp_cksum = 0;
	picmp->icmp_seq = seq;
	picmp->icmp_id = getpid();

	ptime = (struct timeval *)picmp->icmp_data;

	gettimeofday(ptime, NULL);

	picmp->icmp_cksum = get_cksum(picmp);

	if (seq == 1)
		t_send = *ptime;
}

void send_package(int sock_icmp, struct sockaddr_in *dest, int send_order)
{
	set_icmp(send_order);
	int flag = sendto(sock_icmp, send_buff, ICMP_LEN, 0, (struct sockaddr *)dest, sizeof(struct sockaddr_in));
	if (flag < 0)
	{
		perror("sendto");
		return;
	}
}

double get_rtt(struct timeval *recv, struct timeval *send)
{
	struct timeval sub = *recv;

	if ((sub.tv_usec -= send->tv_usec) < 0)
	{
		--(sub.tv_sec);
		sub.tv_usec += 1000000;
	}
	sub.tv_sec -= send->tv_sec;

	return sub.tv_sec * 1000.0 + sub.tv_usec / 1000.0; //转换单位为毫秒
}
void signal_handler(int signo)
{
	double tmp;
	avg /= recv_order;
	tmp = mdev / recv_order - avg * avg;
	mdev = sqrt(tmp);

	if (NULL != host)
		printf("--- %s  ping handler ---\n", host->h_name);
	else
		printf("--- %s  ping handler ---\n", ip_str);

	printf("%d packets transmitted, %d received, %d%% packet loss, time %dms\n",
		   send_order,
		   recv_order,
		   (int)(double)(send_order - recv_order) / send_order * 100,
		   (int)get_rtt(&t_recv, &t_send));
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", min, avg, max, mdev);

	close(sock_icmp);
	exit(0);
}

int unpack(struct timeval *t)
{
	struct ip *ip_ptr = (struct ip *)recv_buff;
	struct icmp *icmp_ptr;
	int ip_hlen = ip_ptr->ip_hl << 2;
	double rtt;

	icmp_ptr = (struct icmp *)(recv_buff + ip_hlen);

	//whether it is a echo reply
	if (icmp_ptr->icmp_type == ICMP_ECHOREPLY && icmp_ptr->icmp_id == getpid())
	{
		struct timeval *send_time = (struct timeval *)icmp_ptr->icmp_data;
		rtt = get_rtt(t, send_time);

		printf("%u bytes from %s: icmp_seq=%u ttl=%u time=%.1f ms\n",
			   ntohs(ip_ptr->ip_len) - ip_hlen,
			   inet_ntoa(ip_ptr->ip_src),
			   icmp_ptr->icmp_seq,
			   ip_ptr->ip_ttl,
			   rtt);

		if (rtt < min || 0 == min)
			min = rtt;
		if (rtt > max)
			max = rtt;
		avg += rtt;
		mdev += rtt * rtt;
		return 0;
	}
	return -1;
}
int recv_package(int sock_icmp, struct sockaddr_in *dest)
{
	int recv_bytes = 0;
	int addr_len = sizeof(struct sockaddr_in);

	struct timeval recv_time;

	signal(SIGALRM, signal_handler);
	alarm(5);

	recv_bytes = recvfrom(sock_icmp, recv_buff, RECV_BUFFER_SIZE, 0, (struct sockaddr *)dest, &addr_len);
	if (recv_bytes < 0)
	{
		perror("recvfrom");
		return 0;
	}

	gettimeofday(&recv_time, NULL);
	t_recv = recv_time;

	if (unpack(&recv_time) == -1)
	{
		return -1;
	}
	recv_order++;
	//return recv_bytes;
}

void ping(int argc, char *argv[])
{
	struct sockaddr_in dest;
	struct protoent *protocol;

	in_addr_t inaddr;

	if (argc < 2)
	{
		fprintf(stderr, "Hostname or IP address is needed\n");
		exit(EXIT_FAILURE);
	}

	if ((protocol = getprotobyname("icmp")) == NULL)
	{
		fprintf(stderr, "%s %d:", __FILE__, __LINE__);
		perror("getprotocolbyname");
		exit(EXIT_FAILURE);
	}

	//create socket
	if ((sock_icmp = socket(PF_INET, SOCK_RAW, protocol->p_proto)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	dest.sin_family = AF_INET;

	if ((inaddr = inet_addr(argv[1])) == INADDR_NONE) //argv[1] is not an ip string
	{
		if ((host = gethostbyname(argv[1])) == NULL)
		{
			herror("gethostbyname");
			exit(EXIT_FAILURE);
		}
		memmove(&dest.sin_addr, host->h_addr_list[0], host->h_length);
	}
	else //argv[1] is an ip string
	{
		memmove(&dest.sin_addr, &inaddr, sizeof(struct in_addr));
	}

	if (host != NULL)
	{
		printf("ping %s\n", host->h_name);
	}
	else
	{
		printf("ping %s\n", argv[1]);
	}

	printf("%s, bytes of data: %d\n", inet_ntoa(dest.sin_addr), ICMP_LEN);

	ip_str = argv[1];
	signal(SIGINT, signal_handler); // Ctrl + C Interupt handler

	while (send_order < SEND_NUM)
	{
		send_package(sock_icmp, &dest, send_order);

		int unpack_ret = recv_package(sock_icmp, &dest);
		if (unpack_ret == -1)
		{
			recv_package(sock_icmp, &dest);
		}
		sleep(1);
		send_order++;
	}

	signal_handler(0);
}
int main(int argc, char *argv[])
{
	ping(argc, argv);
	return 0;
}