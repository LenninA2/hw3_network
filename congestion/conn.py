# simulates one side of tcp connection
# this is used to model congestion (no packet loss)
import random

class Connection:
    def __init__(self, name):
        self.name = name
        self.sent_packets = []
        self.ack_packets = []
        self.ticks = 0

    def get_time(self):
        return self.ticks

    def get_RTT(self):
        return 1

    def send(self, msg):
        self.sent_packets.append((msg, self.ticks + self.get_RTT()))

    def get_acks(self):
        acks = self.ack_packets
        self.ack_packets = []
        return acks

    def tick(self):
        self.ticks += 1

        for packet in self.sent_packets:
            if packet[1] == self.ticks:
                self.ack_packets.append(packet[0])
            elif packet[1] == -1: # packet was dropped, so remove it
                self.sent_packets.remove(packet)

    # print
    def __str__(self):
        return f'{self.name}(tick={self.ticks})'


class PerfectConnection(Connection):
    def __init__(self, RTT=1):
        super().__init__("PerfectConnection")
        self.RTT = RTT

    def get_RTT(self):
        return self.RTT


class VariableRTTConnection(Connection):
    def __init__(self, min_RTT=1, max_RTT=10):
        super().__init__("VariableRTTConnection")
        self.min_RTT = min_RTT
        self.max_RTT = max_RTT

    def get_RTT(self):
        return random.randint(self.min_RTT, self.max_RTT)


class PacketLossConnection(Connection):
    def __init__(self, min_RTT=1, max_RTT=10, packet_loss_prob=0.1):
        super().__init__("PacketLossConnection")
        self.min_RTT = min_RTT
        self.max_RTT = max_RTT
        self.packet_loss_prob = packet_loss_prob
    
    def get_RTT(self):
        # check for packet loss
        if random.random() < self.packet_loss_prob:
            return -1
        else:
            # return random RTT
            return random.randint(self.min_RTT, self.max_RTT)

class CongestedConnection(Connection):
    def __init__(self, congestion_factor=0.1, lookback_window=10):
        super().__init__("CongestedConnection")
        self.congestion_factor = congestion_factor
        self.track_RTT_history = []
        self.lookback_window = lookback_window

    def get_RTT(self):
        self.track_RTT_history.append(self.ticks)

        weighted_RTT = 0
        for tick in self.track_RTT_history:
            if tick > self.ticks - self.lookback_window:
                weighted_RTT += 1
        
        print(weighted_RTT)

        # weight by congestion factor
        RTT = max(1, round(weighted_RTT * self.congestion_factor))
        return RTT
