

import sys

def float_tokens(fp):
    for line in fp:
        for tok in line.split():
            try:
                yield float(tok)
            except ValueError:
                continue

def extract_triangles(path, ids):
    ids = set(ids)
    found = []
    with open(path, "r") as f:
        it = float_tokens(f)
        try:
            n = int(next(it))  
        except StopIteration:
            raise RuntimeError("Пустой файл или нет числа N в первой строке.")
        for i in range(n):
            coords = []
            for _ in range(9):
                try:
                    coords.append(next(it))
                except StopIteration:
                    raise RuntimeError(f"Неожиданный конец файла: треугольник {i}, прочитано {len(coords)} чисел.")
            if i in ids:
                found.append((i, coords))
            if len(found) == len(ids):
                break
    return found

def fmt(x):
    return f"{x:.12g}"

def main():
    if len(sys.argv) < 3:
        print("usage: python3 extract_triangles.py <file.dat> <id1> [id2 ...]")
        sys.exit(1)

    filename = sys.argv[1]
    ids = [int(x) for x in sys.argv[2:]]

    tris = extract_triangles(filename, ids)
    if not tris:
        print("не найдено ни одного треугольника с указанными ID (ID нумеруются с 0)")
        return

    print(f"Найдено {len(tris)} треугольников:\n")
    id2c = {tid: c for tid, c in tris}
    ordered = [(tid, id2c[tid]) for tid in ids if tid in id2c]

    for tid, c in ordered:
        print(f"ID {tid}:")
        print(f"   {fmt(c[0])} {fmt(c[1])} {fmt(c[2])}")
        print(f"   {fmt(c[3])} {fmt(c[4])} {fmt(c[5])}")
        print(f"   {fmt(c[6])} {fmt(c[7])} {fmt(c[8])}\n")

    out_path = "debug_triangles.dat"
    with open(out_path, "w") as out:
        out.write(f"{len(ordered)}\n")
        for _, c in ordered:
            out.write(" ".join(fmt(x) for x in c) + "\n")
    print(f"Сохранено {len(ordered)} треугольников в {out_path}")

if __name__ == "__main__":
    main()



# python3 extract_triangles.py 7.dat 57168 295606 571483 882764
