# Aucun n'import ne doit être fait dans ce fichier


def nombre_entier(n: int) -> str:
    return "S" * n + "0"


def S(n: str) -> str:
    return "S" + n


def addition(a: str, b: str) -> str:
    if a == "0":
        return b
    return S(addition(a[1:], b))


def multiplication(a: str, b: str) -> str:
    if a == "0" or b == "0":
        return "0"
    return addition(b, multiplication(a[1:], b))


def facto_ite(n: int) -> int:
    if n == 0:
        return 1
    res = 1
    for i in range(1, n + 1):
        res *= i
    return res


def facto_rec(n: int) -> int:
    if n == 0:
        return 1
    return n * facto_rec(n - 1)


def fibo_rec(n: int) -> int:
    if n < 2:
        return n
    return fibo_rec(n - 1) + fibo_rec(n - 2)


def fibo_ite(n: int) -> int:
    if n < 2:
        return n
    a, b = 0, 1
    for _ in range(n - 1):
        a, b = b, a + b
    return b


def golden_phi(n: int) -> int:
    if n == 0:
        return 1
    return fibo_ite(n + 1) / fibo_ite(n)

def sqrt5(n: int) -> int:
    phi = golden_phi(n + 1)
    return 2 * phi - 1


def pow(a: float, n: int) -> float:
    return a**n
