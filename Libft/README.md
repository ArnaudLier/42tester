# 42 Libft Tester

To use it, create a symbolic link to the Libft directory in this directory with name `libft`.
Then, run:

```bash
make && ./main
```

If Libft has the bonus functions, run:
```bash
make bonus && ./main
```

Bonus functions' prototypes must be included in `libft.h`.

Things this does not test (yet):
- [ ] malloc "protections"
- [ ] free everything allocated when one fails
  - [ ] lstmap (including new mapped content)
  - [ ] strtrim
  - [ ] strsplit

Made by `alier` @ [42 Mulhouse](https://www.42mulhouse.fr)
(not proud of it given the code quality)
