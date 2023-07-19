from typing import Callable, Union

ReadOnlyBuffer = Union[bytes, bytearray, memoryview]
WriteBuffer = Union[bytearray, memoryview]

class RsyncError(Exception):
    pass

class Hasher:
    def __init__(self, which: str, data: ReadOnlyBuffer = b''): ...
    def update(self, data: ReadOnlyBuffer) -> None: ...
    def reset(self) -> None: ...
    def digest(self) -> bytes: ...
    def hexdigest(self) -> str: ...

    @property
    def digest_size(self) -> int: ...
    @property
    def block_size(self) -> int: ...
    @property
    def name(self) -> str: ...


class Patcher:

    def __init__(self, expected_input_size: int = 0): ...
    def signature_header(self, output: WriteBuffer) -> int: ...
    def sign_block(self, block: ReadOnlyBuffer, output: WriteBuffer) -> int: ...
    def apply_delta_data(self, data: ReadOnlyBuffer, read: Callable[[int, WriteBuffer], int], write: Callable[[ReadOnlyBuffer], None]) -> None: ...
    def finish_delta_data(self) -> None: ...

    @property
    def block_size(self) -> int: ...
    @property
    def total_data_in_delta(self) -> int: ...


class Differ:

    def add_signature_data(self, data: ReadOnlyBuffer) -> None: ...
    def finish_signature_data(self) -> None: ...
    def next_op(self, read: Callable[[WriteBuffer], int], write: Callable[[ReadOnlyBuffer], None]) -> bool: ...


def decode_utf8_buffer(x: ReadOnlyBuffer) -> str: ...
def parse_ftc(x: Union[str, ReadOnlyBuffer], callback: Callable[[memoryview, memoryview], None]) -> None: ...
