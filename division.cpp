//Alumna: Mariana Paula Caceres Urquizo 

#include <iostream>
#include <bitset>

template <long long unsigned int n>
int bitsToPositiveInt(std::bitset<n> bits)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        if (bits[i])
        {
            int pow2 = 1;
            for (int j = 0; j < i; j++)
            {
                pow2 *= 2;
            }
            sum += pow2;
        }
    }
    return sum;
}

template <unsigned int n>
std::bitset<n> sum(std::bitset<n> lhs, std::bitset<n> rhs)
{
    //Retorna la suma entre dos bitsets (probablemente puede ser optimisado)
    std::bitset<n> bits;
    std::bitset<n> carry = 0;

    for (int i = 0; i < n; i++)
    {
        //XOR para determinar si es 0 o 1
        bits.set(i, lhs[i] ^ rhs[i]);
        //El bit se invierte si es 1 el carry
        if (carry[i])
        {
            bits.flip(i);
            //Solo se guarda un carry si esta dentro de los limites
            if (i != n - 1)
                //Se calcula el carry con OR
                carry.set(i + 1, lhs[i] | rhs[i]);
        }
        else
        {
            //Solo se guarda un carry si esta dentro de los limites
            if (i != n - 1)
                //Se calcula el carry con AND
                carry.set(i + 1, lhs[i] & rhs[i]);
        }
    }
    return bits;
}

template <unsigned int n>
std::bitset<n> diff(std::bitset<n> lhs, std::bitset<n> rhs)
{
    //Retorna la resta entre dos bitsets (usando la suma)
    //SOLO si "lhs" es mayor a "rhs"

    //Extender por un bit lhs y rhs
    std::bitset<n + 1> newLhs;
    std::bitset<n + 1> newRhs;
    for (int i = 0; i < n; i++)
    {
        newLhs.set(i, lhs[i]);
        newRhs.set(i, rhs[i]);
    }
    newLhs.reset(n);
    newRhs.reset(n);

    //Calcular el complemento a 2 de rhs
    newRhs.flip();
    newRhs = sum<n + 1>(newRhs, std::bitset<n + 1>(1));

    //Sumar y retornar la resta si lhs > rhs 
    auto bits = sum<n + 1>(newLhs, newRhs);
    if (bits[n])
    {
        return lhs;
    }
    else
    {
        //Reducir en un bit el resultado
        std::bitset<n> newBits;
        for (int i = 0; i < n; i++)
        {
            newBits.set(i, bits[i]);
        }
        return newBits;
    }
}

template <unsigned int n>
void shiftLeft(std::bitset<n>& lhs, std::bitset<n>& rhs)
{
    //Mueve todos los bits a la izquierda
    //EL NUEVO BIT ES 0
    for (int i = n - 1; i > 0; i--)
    {
        lhs.set(i, lhs[i - 1]);
    }
    lhs.set(0, rhs[n - 1]);
    for (int i = n - 1; i > 0; i--)
    {
        rhs.set(i, rhs[i - 1]);
    }
    rhs.reset(0);
}

template <unsigned int n>
std::bitset<n> division(std::bitset<n> Q, std::bitset<n> M, std::bitset<n>& remainderDiv)
{
    std::bitset<n> A = 0;      //palabra auxiliar de n bits, empieza en 0
    unsigned int count = n;

    for (int i = n; i > 0; i--)
    {
        shiftLeft<n>(A, Q);
        if (bitsToPositiveInt(A) >= bitsToPositiveInt(M))
            Q.set(0, true);      //nuevo bit (derecha) = 1
        A = diff<n>(A, M);       //solo resta si A es mayor a M
        count -= 1;
    }
    remainderDiv = A;
    return Q;
}

int main()
{
    //division<n bits>(dividendo, divisor)
    //3 ejemplos con tres bitsizes distintos

    unsigned int Q;
    unsigned int M;
    std::cout << "Ejemplo 1: 4 bits (numeros del 0 al 15)\n";
    std::cout << "Ingrese el dividendo y el divisor: ";
    std::cin >> Q >> M;

    std::bitset<4> remainderDiv1;
    auto div1 = division<4>(Q, M, remainderDiv1);
    std::cout << "Cociente: " << div1 << ": " << bitsToPositiveInt(div1) << '\n';
    std::cout << "Residuo: " << remainderDiv1 << ": " << bitsToPositiveInt(remainderDiv1) << "\n\n";

    
    std::cout << "Ejemplo 2: 8 bits (numeros del 0 al 255)\n";
    std::cout << "Ingrese el dividendo y el divisor: ";
    std::cin >> Q >> M;

    std::bitset<8> remainderDiv2;
    auto div2 = division<8>(Q, M, remainderDiv2);
    std::cout << "Cociente: " << div2 << ": " << bitsToPositiveInt(div2) << '\n';
    std::cout << "Residuo: " << remainderDiv2 << ": " << bitsToPositiveInt(remainderDiv2) << "\n\n";

    
    std::cout << "Ejemplo 3: 16 bits (numeros del 0 al 65535)\n";
    std::cout << "Ingrese el dividendo y el divisor: ";
    std::cin >> Q >> M;

    std::bitset<16> remainderDiv3;
    auto div3 = division<16>(Q, M, remainderDiv3);
    std::cout << "Cociente: " << div3 << ": " << bitsToPositiveInt(div3) << '\n';
    std::cout << "Residuo: " << remainderDiv3 << ": " << bitsToPositiveInt(remainderDiv3) << "\n\n";

    return 0;
}