import time
import threading

class Snowflake:
    def __init__(self, machine_id, epoch=1420070400000):
        # Constants for bit lengths
        self.timestamp_bits = 41
        self.machine_id_bits = 10
        self.sequence_bits = 12

        # Maximum values based on bit lengths
        self.max_machine_id = -1 ^ (-1 << self.machine_id_bits)
        self.max_sequence = -1 ^ (-1 << self.sequence_bits)

        # Bit shifts
        self.timestamp_shift = self.machine_id_bits + self.sequence_bits
        self.machine_id_shift = self.sequence_bits

        # Epoch (start time in milliseconds)
        self.epoch = epoch

        # Machine ID
        if machine_id < 0 or machine_id > self.max_machine_id:
            raise ValueError(f"Machine ID must be between 0 and {self.max_machine_id}")
        self.machine_id = machine_id

        # Sequence number
        self.sequence = 0
        self.last_timestamp = -1

        # Lock for thread safety
        self.lock = threading.Lock()

    def _current_timestamp(self):
        return int(time.time() * 1000)

    def _wait_for_next_millisecond(self, last_timestamp):
        timestamp = self._current_timestamp()
        while timestamp <= last_timestamp:
            timestamp = self._current_timestamp()
        return timestamp

    def generate_id(self):
        with self.lock:
            timestamp = self._current_timestamp()

            if timestamp < self.last_timestamp:
                raise Exception("Clock moved backwards. Refusing to generate id")

            if timestamp == self.last_timestamp:
                self.sequence = (self.sequence + 1) & self.max_sequence
                if self.sequence == 0:
                    timestamp = self._wait_for_next_millisecond(self.last_timestamp)
            else:
                self.sequence = 0

            self.last_timestamp = timestamp

            id = ((timestamp - self.epoch) << self.timestamp_shift) | \
                 (self.machine_id << self.machine_id_shift) | \
                 self.sequence

            return id

# Example usage:
# The Snowflake ID generation algorithm is used to generate unique identifiers
# in a distributed system. It ensures that IDs are unique across multiple machines
# and over time. The generated ID is a 64-bit integer composed of a timestamp,
# machine ID, and sequence number.
snowflake = Snowflake(machine_id=1)
unique_id = snowflake.generate_id()
print(unique_id)

# Use cases for the Snowflake ID generation algorithm:
# 1. Distributed databases: Ensuring unique primary keys across multiple database nodes.
# 2. Message queues: Generating unique message IDs in a distributed messaging system.
# 3. Microservices: Creating unique request or transaction IDs across different services.
# 4. Logging: Generating unique log entry IDs for tracing and debugging purposes.
# 5. Caching: Creating unique cache keys for distributed caching systems.
