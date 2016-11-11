char find_operator(unsigned char *buffer)
{
	int i = 0;
	while(buffer[i] != (unsigned char)('\0'))
	{
		if(buffer[i] <= 0x39 && buffer[i] >= 0x30)
			{i += 1;}
		else
			{break;}
	}
	return (char)buffer[i];
}


void construct_string(unsigned int operand , unsigned char * s_operand)
{
	int cp_operand = operand;
	int i = 0 , j;
	if(cp_operand < 0)
	{
		s_operand[i++] = 45;
		cp_operand *= -1;
	}
	while(cp_operand != 0)
	{
		s_operand[i] = (unsigned char)((cp_operand % 10) + 48);
		cp_operand /= 10;
		i++;
	}
	if(s_operand[0] != '-')
	{
		for(j = 0; j < (i >> 1) ; j++)
		{
			s_operand[j] = s_operand[j] + s_operand[i - j - 1] - (s_operand[i - j - 1] = s_operand[j]);
		}
		s_operand[i] = '\0';
	}
	else
	{
		for(j = 1; j < (i >> 1) ; j++)
		{
			s_operand[j] = s_operand[j] + s_operand[i - j] - (s_operand[i - j] = s_operand[j]);
		}
		s_operand[i] = '\0';		
	}
}

void extract_numbers(unsigned char *buffer , int operand_count , unsigned int *operand_1 , unsigned int *operand_2)
{
	int i = 0 ; 
	unsigned int operand_1_copy = 0;
	*operand_1 = 0;
	while(buffer[i] != '\0')
	{
		
		if((buffer[i] <= 0x39 && buffer[i] >= 0x30))
		{
			operand_1_copy *= 10;
			operand_1_copy += (buffer[i] - 48);
			i++;
		}
		else break;
	}
	(*operand_1 = operand_1_copy);
	if(operand_count != 1)
	{
		*operand_2 = 0;
		while(buffer[i] != '\0' && (buffer[i] <= 0x39 && buffer[i] >= 0x30) <= 0){i++;}
		while(buffer[i] != '\0')
		{
			if((buffer[i] <= 0x39 && buffer[i] >= 0x30))
			{
				*operand_2 *= 10;
				*operand_2 += (buffer[i] - 48);
				i++;
			}
			else break;
		}
	}
	return;	
}

void addition(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned int operand_1 = 0, operand_2 = 0 , result;
	extract_numbers(buffer , 2 , &operand_1 , &operand_2);
	result = operand_1 + operand_2;
	construct_string(result , s_operand);
}

void subtraction(unsigned char *buffer ,unsigned char *s_operand)
{
	unsigned int operand_1 , operand_2, result;
	extract_numbers(buffer ,2 , &operand_1 , &operand_2);
	result = operand_1 - operand_2;
	construct_string(result , s_operand);
}

void multiplication(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned int operand_1 , operand_2 , result;
	extract_numbers(buffer, 2 , &operand_1 , &operand_2);
	result = operand_1 * operand_2;
	construct_string(result,s_operand);
}

void modulo(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned int operand_1 , operand_2 , result;
	extract_numbers(buffer ,2 , &operand_1 , &operand_2);
	result = operand_1 % operand_2;
	construct_string(result , s_operand);
}

void exponent(unsigned char *buffer , unsigned char *s_operand)
{
	int i;
	unsigned int operand_1 , operand_2 , result;
	extract_numbers(buffer ,2 , &operand_1 , &operand_2);
	result = 1;
	for(i = 0 ; i < operand_2 ; i++)
	{
		result *= operand_1;
	}
	construct_string(result , s_operand);
}

void division(unsigned char *buffer ,unsigned char *s_operand)
{
	unsigned int operand_1 , operand_2;
	extract_numbers(buffer ,2, &operand_1 , &operand_2);
	if(operand_2 == 0 )
	{
		s_operand[0] = 'E';
		s_operand[1] = 'R';
		s_operand[2] = 'R';
		s_operand[3] = '\0';				
	}
	else
	{
		unsigned int result = operand_1 / operand_2;
		construct_string(result , s_operand);
	}
}

void factorial(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned int operand_1 , operand_2;
	unsigned int result = 1 , i = 1;
	extract_numbers(buffer , 1 , &operand_1 , &operand_2);
	while(i <= operand_1)
	{
		result *= i;
		i++;
	}
	construct_string(result , s_operand);
}

void evaluate(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned char operator ;
	operator = find_operator(buffer);
	switch(operator)
	{
		case '+' : {addition(buffer,s_operand); break;}
		case '-' : {subtraction(buffer,s_operand); break;}
		case '*' : {multiplication(buffer,s_operand); break;}
		case '/' : {division(buffer,s_operand); break;}
		case '%' : {modulo(buffer,s_operand); break;}
		case '^' : {exponent(buffer,s_operand); break;}
		case '!' : {factorial(buffer,s_operand); break;}
//		case 'S' : {return sine(buffer); break;}
//		case 'C' : {return cosine(buffer); break;}
//		case 'T' : {return tangent(buffer); break;}
//		default : {return normalize(buffer); break;}
	}
}	
