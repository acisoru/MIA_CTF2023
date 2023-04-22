import asyncio
import string


async def handle_client(reader, writer):
    welcome_message = "Добро пожаловать на наш крутой сервер шифрования!\n1) Зашифровать сообщение\n2) Выйти\n> "
    writer.write(welcome_message.encode())
    await writer.drain()

    while True:
        data = await reader.readline()
        option = data.decode().strip()

        if option == '1':
            writer.write("Введите сообщение: ".encode())
            await writer.drain()

            data = await reader.readline()
            message = data.decode().strip()
            alphabet = string.ascii_letters + string.digits
            key = ['1', '7', 'I', 'a', 'f', 'r', 'Y', 'w', 'c', 'b', 'T', 'n', 'D', 'v', 'J', 'V', 'z', 'j', 'Z', 'S', 'h', 'G', '4', 'e', 'q', 'C', 'p', 'x', '8', '2', 'O', 'K', 'H', 'm', '0', '6', 'g', 'k', 's', 'R', 'F', 't', 'L', 'u', 'X', 'N', 'U', '5', 'M', 'd', 'i', 'P', '3', '9', 'W', 'Q', 'l', 'E', 'B', 'o', 'A', 'y']

            encrypted_message = ''

            for char in message.lower():
                if char in alphabet:
                    index = alphabet.index(char)
                    encrypted_message += key[index]
                else:
                    encrypted_message += char
            writer.write(
                f"Зашифрованное сообщение: {encrypted_message}\n1) Зашифровать сообщение\n2) Выйти\n> ".encode())
            await writer.drain()

        elif option == '2':
            writer.close()
            break

        else:
            writer.write("Неправильная опция\n1) Зашифровать сообщение\n2) Выйти\n> ".encode())
            await writer.drain()


async def main():
    server = await asyncio.start_server(handle_client, '0.0.0.0', 9090)

    async with server:
        await server.serve_forever()


if __name__ == '__main__':
    asyncio.run(main())