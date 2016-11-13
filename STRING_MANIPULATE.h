#include <math.h>

unsigned int modify_float_expr = 0x00;

char find_operator(unsigned char *buffer)
{
	int i = 0;
	unsigned char ret_operator;
	int t_count = 0;
	while(buffer[i] != '\0')
	{
		if(buffer[i] == '.')
			{
				modify_float_expr = 0x01;
				i++;
			}		
		else if((buffer[i] <= 0x39 && buffer[i] >= 0x30)||(buffer[i] <= 0x46 && buffer[i] >= 0x41))
			{i += 1;}
		else
			{ret_operator = buffer[i]; break;}
	}
	return (char)ret_operator;
}

void construct_string_floating(float operand , unsigned char * s_operand)
{
	float cp_operand = operand;
	int i = 0 , j;
	if(cp_operand < 0)
	{
		s_operand[i++] = 45;
		cp_operand *= -1;
	}
	while((int)cp_operand != 0)
	{
		s_operand[i] = (unsigned char)((((int)cp_operand) % 10) + 48);
		cp_operand /= 10;
		i++;
	}
	if(s_operand[0] != '-')
	{
		for(j = 0; j < (i >> 1) ; j++)
		{
			s_operand[j] = s_operand[j] + s_operand[i - j - 1] - (s_operand[i - j - 1] = s_operand[j]);
		}
	}
	else
	{
		for(j = 1; j < (i >> 1) ; j++)
		{
			s_operand[j] = s_operand[j] + s_operand[i - j] - (s_operand[i - j] = s_operand[j]);
		}
	}
	s_operand[i++] = '.';
	s_operand[i++] = ((int)((operand - (int)operand)*10) + 48);
	s_operand[i++] = (int)((operand - (int)operand)*100)%10 + 48;	
	s_operand[i++] = (int)((operand - (int)operand)*1000)%10 + 48;		
	s_operand[i++] = '\0';
	return;
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

void extract_numbers_floating(unsigned char *buffer , int operand_count ,float *operand_1 ,float *operand_2)
{
	int i = 0 ; 
	float operand_1_copy = 0;
	*operand_1 = 0;
	unsigned int trail = 1;
	unsigned int point_encounter = 0x00;
	while(buffer[i] != '\0')
	{
		if(buffer[i] <= 0x39 && buffer[i] >= 0x30)
		{
			if(point_encounter == 0x00)
			{
				operand_1_copy *= 10;
				operand_1_copy += (buffer[i] - 48);
				i++;
			}
			else
			{
				trail *= 10;
				operand_1_copy += (float)(buffer[i] - 48)/(float)(trail);
				i++;
			}
		}
		else if(buffer[i] == '.')
		{
			point_encounter = 0x01;
			i++;
		}
		else break;
	}
	*operand_1 = operand_1_copy;
	if(operand_count != 1)
	{
		trail = 1;
		point_encounter = 0x00;
		*operand_2 = 0;
		while(buffer[i] != '\0' && (buffer[i] <= 0x39 && buffer[i] >= 0x30) <= 0){i++;}
		while(buffer[i] != '\0')
		{
			if(buffer[i] <= 0x39 && buffer[i] >= 0x30)
			{
				if(!point_encounter)
				{
					*operand_2 *= 10;
					*operand_2 += (buffer[i] - 48);
					i++;
				}
				else
				{
					trail *= 10;
					*operand_2 += (float)(buffer[i] - 48)/(trail);
					i++;
				}
			}
			else if(buffer[i] == '.')
			{
				point_encounter = 0x01;
				i++;
			}
			else break;
		}		
	}
	point_encounter = 0x00;
	return;
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
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	if(modify_float_expr)
	{
		extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
		_f_result = _f_operand_1 + _f_operand_2;
		construct_string_floating(_f_result , s_operand);
	}
	else
	{
		extract_numbers(buffer , 2 , &operand_1 , &operand_2);
		result = operand_1 + operand_2;
		construct_string(result , s_operand);
	}
}

void subtraction(unsigned char *buffer ,unsigned char *s_operand)
{
	unsigned int operand_1 , operand_2, result;
	float _f_operand_1 = 0.00, _f_operand_2 = 0.00 ,_f_result;
	if(modify_float_expr)
	{
		extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
		_f_result = _f_operand_1 - _f_operand_2;
		construct_string_floating(_f_result , s_operand);		
	}
	else
	{
		extract_numbers(buffer ,2 , &operand_1 , &operand_2);
		result = operand_1 - operand_2;
		construct_string(result , s_operand);
	}
}

void multiplication(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned int operand_1 , operand_2 , result;
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	if(modify_float_expr)
	{
		extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
		_f_result = _f_operand_1 * _f_operand_2;
		construct_string_floating(_f_result , s_operand);		
	}
	else
	{
		extract_numbers(buffer, 2 , &operand_1 , &operand_2);
		result = operand_1 * operand_2;
		construct_string(result,s_operand);
	}
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
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	if(modify_float_expr)
	{
		extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
		_f_result = pow(_f_operand_1 , _f_operand_2);
		construct_string_floating(_f_result , s_operand);		
	}
	else
	{
		extract_numbers(buffer ,2 , &operand_1 , &operand_2);
		result = 1;
		for(i = 0 ; i < operand_2 ; i++)
		{
			result *= operand_1;
		}
		construct_string(result , s_operand);
	}
}

void division(unsigned char *buffer ,unsigned char *s_operand)
{
	unsigned int operand_1 , operand_2;
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	if(modify_float_expr)
	{
		extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
		if(_f_operand_2 == 0.00)
		{
			s_operand[0] = 'E';
			s_operand[1] = 'R';
			s_operand[2] = 'R';
			s_operand[3] = 'O';
			s_operand[4] = 'R';
			s_operand[5] = '\0';			
		}
		else
		{
			_f_result = _f_operand_1 / _f_operand_2;
			construct_string_floating(_f_result , s_operand);
		}
	}
	else
	{
		extract_numbers(buffer ,2, &operand_1 , &operand_2);
		if(operand_2 == 0 )
		{
			s_operand[0] = 'E';
			s_operand[1] = 'R';
			s_operand[2] = 'R';
			s_operand[3] = 'O';
			s_operand[4] = 'R';
			s_operand[5] = '\0';				
		}
		else
		{
			unsigned int result = operand_1 / operand_2;
			construct_string(result , s_operand);
		}

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

void sine(unsigned char *buffer, unsigned char *s_operand)
{
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
	_f_result = sin(_f_operand_1);
	construct_string_floating(_f_result , s_operand);
}

void sine_inverse(unsigned char *buffer, unsigned char *s_operand)
{
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 1 , &_f_operand_1 , &_f_operand_2);
	_f_result = asin(_f_operand_1);
	construct_string_floating(_f_result, s_operand);
}

void cosine(unsigned char *buffer, unsigned char *s_operand)
{
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
	_f_result = cos(_f_operand_1);
	construct_string_floating(_f_result , s_operand);
}

void cosine_inverse(unsigned char *buffer , unsigned char *s_operand)
{
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
	_f_result = acos(_f_operand_1);
	construct_string_floating(_f_result , s_operand);	
}

void tangent(unsigned char *buffer, unsigned char *s_operand)
{
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
	_f_result = tan(_f_operand_1);
	construct_string_floating(_f_result , s_operand);
}

void tangent_inverse(unsigned char *buffer, unsigned char *s_operand)
{
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 2 , &_f_operand_1 , &_f_operand_2);
	_f_result = atan(_f_operand_1);
	construct_string_floating(_f_result , s_operand);	
}

void nlog(unsigned char *buffer, unsigned char *s_operand)
{
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 1 , &_f_operand_1 , &_f_operand_2);
	_f_result = log(_f_operand_1);
	construct_string_floating(_f_result , s_operand);
}

void b_10_log(unsigned char *buffer , unsigned char *s_operand)
{
	float _f_operand_1 = 0.00 , _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 1 , &_f_operand_1 , &_f_operand_2);
	_f_result = log10(_f_operand_1);
	construct_string_floating(_f_result , s_operand);
}

void natural_exponent(unsigned char *buffer , unsigned char *s_operand)
{
	float _f_operand_1 = 0.00, _f_operand_2 = 0.00 , _f_result;
	extract_numbers_floating(buffer , 1 , &_f_operand_1 , &_f_operand_2);
	_f_result = exp(_f_operand_1);
	construct_string_floating(_f_result , s_operand);
}

void random_number(unsigned char *buffer, unsigned char *s_operand)
{
	unsigned int result;
	result = rand();
	construct_string(result , s_operand);	
}

void hex_to_dec(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned int result = 0;
	int i = 0;
	while(buffer[i] != 'h'){i++;}
	i--;
	int base = 1;
	while(i >= 0)
	{
		if(buffer[i] <= 39 && buffer[i] >= 30)
			result += (buffer[i] - 48)*base;
		else
			result += (buffer[i] - 65 + 10)*base;
		base *= 16;
		i--;
	}
	construct_string(result , s_operand);
}

void oct_to_dec(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned int result = 0;
	int i;
	while(buffer[i] != 'o'){i++;}
	i--;
	int base = 1;
	while(i >= 0)
	{
		result += (buffer[i] - 48)*base;
		base *= 8;
		i--;
	}
	construct_string(result , s_operand);
}

void bin_to_dec(unsigned char *buffer , unsigned char *s_operand)
{
	unsigned int result = 0;
	int i;
	while(buffer[i] != 'b'){i++;}
	i--;
	int base = 1;
	while(i >= 0)
	{
		result += (buffer[i] - 48)*base;
		base = base << 1;
		i--;
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
		case 's' : {sine(buffer, s_operand); modify_float_expr = 0x01; break;}
		case 'c' : {cosine(buffer, s_operand); modify_float_expr = 0x01; break;}
		case 't' : {tangent(buffer, s_operand); modify_float_expr = 0x01; break; }
		case 'l' : {nlog(buffer, s_operand); modify_float_expr = 0x01; break;}
		case 'g' : {b_10_log(buffer, s_operand); modify_float_expr = 0x01; break;}
		case 'e' : {natural_exponent(buffer, s_operand); modify_float_expr = 0x01; break;}
		case 'S' : {sine_inverse(buffer, s_operand); modify_float_expr = 0x01; break;}
		case '#' : {cosine_inverse(buffer, s_operand); modify_float_expr = 0x01; break;}
		case 'T' : {tangent_inverse(buffer, s_operand); modify_float_expr = 0x01; break;}
		case 'R' : {random_number(buffer, s_operand); break;}
		case 'h' : {hex_to_dec(buffer, s_operand);break;}
		case 'o' : {oct_to_dec(buffer, s_operand);break;}
		case 'b' : {bin_to_dec(buffer, s_operand);break;}
//		default : {return normalize(buffer); break;}
	}
}	
