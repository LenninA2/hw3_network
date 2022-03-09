import conn
from packet_data import packets

if __name__=='__main__':
    pconn = conn.CongestedConnection(congestion_factor=10, lookback_window=10)
    ack_packets = set()
    send_idx = 0

    # TODO: implement this!
    # 1. packets do not drop, but are delayed based on how many packets are sent recently
    # 2. Your goal is to minimize the number of ticks required to send all packets
    # 3. Start by seeing what happens if you send a single packet per tick.
