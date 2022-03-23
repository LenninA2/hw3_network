class Packet:
    def __init__(self, src_ip_addr, dst_ip_addr, src_port, dst_port):
        self.src_ip_addr = src_ip_addr
        self.dst_ip_addr = dst_ip_addr
        self.src_port = src_port
        self.dst_port = dst_port


class Action:
    def __init__(self, action_type, action_value):
        # action_type: 'forward' or 'drop'
        self.action_type = action_type
        # for 'forward', action_value is the next router
        self.action_value = action_value


class SDNRouter:
    def __init__(self, ip_address, rules):
        self.ip_address = ip_address
        # rules is a list of (Match functions, Action) tuples
        self.rules = rules

    def forward(self, packet):
        for match_func, action in self.rules:
            if match_func(packet):
                if action.action_type == 'forward':
                    return action.action_value
                elif action.action_type == 'drop':
                    return 'drop'


def get_packet_route(packet, network_map, max_hops=8):
    hops = [packet.src_ip_addr]
    for i in range(max_hops):
        current_hop = hops[-1]

        # if we have too many hops, we have a loop (drop packet)
        if i == max_hops - 1:
            hops.append('x (dropped)')
            break

        # if current_hop is dst, return hops
        elif current_hop == packet.dst_ip_addr:
            break

        # else, find the next hop using the router forwarding table
        else:   
            next_hop = network_map[current_hop].forward(packet.dst_ip_addr)
            if next_hop == 'drop': # SDNRouter dropped the packet
                hops.append('x (dropped)')
                break
            hops.append(next_hop)
    
    # print the route
    print('Route: ', end='')
    for hop in hops[:-1]:
        print(f'{hop} -> ', end='')
    print(hops[-1])

    return hops


if __name__=='__main__':
    # network map for first network example
    network_map = {
        '0000': SDNRouter('0000', []),
        '0001': SDNRouter('0001', []),
        '0010': SDNRouter('0010', []),
        '0011': SDNRouter('0011', []),
        '0100': SDNRouter('0100', []),
    }

    # HTTPS request packet
    p1 = Packet('0000', '0001', '34213', '8080')

    # HTTPS response packet
    p2 = Packet('0001', '0000', '8080', '34213')

    # Here are some get_packet_route examples
    # These currently drop since the Router.forward() method is not implemented
    get_packet_route('0000', '0001', network_map)
    get_packet_route('0010', '0000', network_map)
