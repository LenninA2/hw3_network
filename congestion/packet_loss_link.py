import conn
from packet_data import packets

if __name__=='__main__':
    pconn = conn.PacketLossConnection(min_RTT=1, max_RTT=10, packet_loss_prob=0.1)
    ack_packets = set()
    send_idx = 0

    # TODO: implement this!
    # 1. need to track when packets are sent (use conn.get_time() to get the current tick)
    # 2. need to estimate the timeout. Think about what extra parameters you will need here.
    # 3. what do you need to do to handle timeouts?
