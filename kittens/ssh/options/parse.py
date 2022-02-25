# generated by gen-config.py DO NOT edit

import typing
from kittens.ssh.options.utils import hostname
from kitty.conf.utils import merge_dicts


class Parser:

    def hostname(self, val: str, ans: typing.Dict[str, typing.Any]) -> None:
        hostname(val, ans)


def create_result_dict() -> typing.Dict[str, typing.Any]:
    return {
    }


actions: typing.FrozenSet[str] = frozenset(())


def merge_result_dicts(defaults: typing.Dict[str, typing.Any], vals: typing.Dict[str, typing.Any]) -> typing.Dict[str, typing.Any]:
    ans = {}
    for k, v in defaults.items():
        if isinstance(v, dict):
            ans[k] = merge_dicts(v, vals.get(k, {}))
        elif k in actions:
            ans[k] = v + vals.get(k, [])
        else:
            ans[k] = vals.get(k, v)
    return ans


parser = Parser()


def parse_conf_item(key: str, val: str, ans: typing.Dict[str, typing.Any]) -> bool:
    func = getattr(parser, key, None)
    if func is not None:
        func(val, ans)
        return True
    return False
