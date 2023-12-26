'use client'

import {
    Flex,
    Box,
    Card,
    CardBody,
    CardHeader,
    Heading,
    Grid,
    GridItem,
    Text,
    Input,
    CardFooter,
    Button,
    Menu,
    MenuButton,
    MenuList,
    MenuItem,
    Textarea,
    useToast,
} from '@chakra-ui/react';
import { ChevronDownIcon } from '@chakra-ui/icons';
import { useState } from 'react';
import config from '@/app/config';


export default function Page() {
    const [show, setShow] = useState(false);

    const [username, setUsername] = useState('');
    const [realName, setRealName] = useState('');
    const [password, setPassword] = useState('');
    const [confirmPassword, setConfirmPassword] = useState('');
    const [identityType, setIdentityType] = useState('');
    const [identityNumber, setIdentityNumber] = useState('');
    const [phoneNumber, setPhoneNumber] = useState('');
    const [country, setCountry] = useState('');
    const [province, setProvince] = useState('');
    const [city, setCity] = useState('');
    const [introduction, setIntroduction] = useState('');

    const [isUsernameInvalid, setIsUsernameInvalid] = useState(false);
    const [isRealNameInvalid, setIsRealNameInvalid] = useState(false);
    const [isPasswordInvalid, setIsPasswordInvalid] = useState(false);
    const [isConfirmPasswordInvalid, setIsConfirmPasswordInvalid] = useState(false);
    const [isIdentityNumberInvalid, setIsIdentityNumberInvalid] = useState(false);
    const [isPhoneNumberInvalid, setIsPhoneNumberInvalid] = useState(false);
    const [isCountryInvalid, setIsCountryInvalid] = useState(false);
    const [isProvinceInvalid, setIsProvinceInvalid] = useState(false);
    const [isCityInvalid, setIsCityInvalid] = useState(false);
    const [isIntroductionInvalid, setIsIntroductionInvalid] = useState(false);

    const identityTypes = {
        '居民身份证': 'IdCard',
        '护照': 'Passport',
    }

    const toast = useToast()
    const handleSubmit = async () => {
        if (username === '' || username.length > 20) {
            toast({
                title: '用户名不合法',
                description: "用户名不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsUsernameInvalid(true);
            return;
        }
        if (realName === '' || realName.length > 20) {
            toast({
                title: '真实姓名不合法',
                description: "真实姓名不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsRealNameInvalid(true);
            return;
        }
        // password must contain 2 numbers, and is not allowed to only contain uppercase or lowercase letters
        let digitCount = 0;
        for (let i = 0; i < password.length; i++) {
            if (password[i] >= '0' && password[i] <= '9') {
                digitCount++;
            }
        }
        if (digitCount < 2) {
            toast({
                title: '密码不合法',
                description: "密码必须包含至少2个数字",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsPasswordInvalid(true);
            return;
        }
        if (password.toUpperCase() === password || password.toLowerCase() === password) {
            toast({
                title: '密码不合法',
                description: "密码不能只包含大写或小写字母",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsPasswordInvalid(true);
            return;
        }

        if (password < 6 || password.length > 20) {
            toast({
                title: '密码不合法',
                description: "密码长度不能小于6且不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsPasswordInvalid(true);
            return;
        }
        if (password !== confirmPassword) {
            toast({
                title: '密码不一致',
                description: "请确认两次输入的密码一致",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsConfirmPasswordInvalid(true);
            return;
        }
        if (identityNumber === '' || identityNumber.length > 20) {
            toast({
                title: '证件号不合法',
                description: "证件号不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsIdentityNumberInvalid(true);
            return;
        }
        if (phoneNumber === '' || phoneNumber.length > 20) {
            toast({
                title: '电话号码不合法',
                description: "电话号码不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsPhoneNumberInvalid(true);
            return;
        }
        if (country === '' || country.length > 20) {
            toast({
                title: '国家不合法',
                description: "国家不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsCountryInvalid(true);
            return;
        }
        if (province === '' || province.length > 20) {
            toast({
                title: '省份不合法',
                description: "省份不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsProvinceInvalid(true);
            return;
        }
        if (city === '' || city.length > 20) {
            toast({
                title: '市不合法',
                description: "市不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsCityInvalid(true);
            return;
        }
        if (introduction.length > 200) {
            toast({
                title: '个人简介不合法',
                description: "个人简介长度不能超过200",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsIntroductionInvalid(true);
            return;
        }

        console.log(JSON.stringify({
            username: username,
            password: password,
            realName: realName,
            documentType: identityTypes[identityType],
            documentNumber: identityNumber,
            phoneNumber: phoneNumber,
            bio: introduction,
            region: city,
            district: province,
            country: country,
        }))
        const res = await fetch(`${config.serverIp}/users`, {
            method: 'POST',
            body: JSON.stringify({
                username: username,
                password: password,
                realName: realName,
                documentType: identityTypes[identityType],
                documentNumber: identityNumber,
                phoneNumber: phoneNumber,
                bio: introduction,
                region: city,
                district: province,
                country: country,
            }),
            headers: { "Content-Type": "application/json" }
        })
        const user = await res.json()

        // If no error and we have user data, return it
        if (res.ok && user) {
            toast({
                title: '注册成功',
                description: "即将跳转至主界面",
                status: 'success',
                duration: 2000,
                isClosable: true,
            })
            const timer = setTimeout(() => {
                window.location.href = '/';
            }, 2000)  
            return;
        }
        // Return null if user data could not be retrieved
        else {
            toast({
                title: '注册失败',
                description: `state: ${res.status}, message: ${user.message}`,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
    };

    return (
        <Card align='center'>
            <CardHeader>
                <Heading size='lg' >注册</Heading>
            </CardHeader>

            <CardBody>
                <Grid templateRows='repeat(8, 1fr)' templateColumns='repeat(6, 1fr)' gap={1}>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>用户名</Text>
                        <Input
                            value={username}
                            onChange={(e) => setUsername(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isUsernameInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>真实姓名</Text>
                        <Input
                            value={realName}
                            onChange={(e) => setRealName(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isRealNameInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>密码</Text>
                        <Input
                            value={password}
                            type={show ? 'text' : 'password'}
                            onChange={(e) => setPassword(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isPasswordInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>确认密码</Text>
                        <Input
                            value={confirmPassword}
                            type={show ? 'text' : 'password'}
                            onChange={(e) => setConfirmPassword(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isConfirmPasswordInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>证件类型</Text>
                        <Menu>
                            <MenuButton w='full' as={Button} rightIcon={<ChevronDownIcon />} >
                                <Flex justify='left'>
                                    {identityType}
                                </Flex>
                            </MenuButton>
                            <MenuList>
                                <MenuItem onClick={() => setIdentityType('居民身份证')}>居民身份证</MenuItem>
                                <MenuItem onClick={() => setIdentityType('护照')}>护照</MenuItem>
                            </MenuList>
                        </Menu>
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>证件号</Text>
                        <Input
                            value={identityNumber}
                            onChange={(e) => setIdentityNumber(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isIdentityNumberInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                        <Text>电话号码</Text>
                        <Input
                            value={phoneNumber}
                            onChange={(e) => setPhoneNumber(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isPhoneNumberInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={3} p='4'>
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={2} p='4'>
                        <Text>国家</Text>
                        <Input
                            value={country}
                            onChange={(e) => setCountry(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isCountryInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={2} p='4'>
                        <Text>省份</Text>
                        <Input
                            value={province}
                            onChange={(e) => setProvince(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isProvinceInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={1} colSpan={2} p='4'>
                        <Text>市</Text>
                        <Input
                            value={city}
                            onChange={(e) => setCity(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isCityInvalid}
                        />
                    </GridItem>
                    <GridItem rowSpan={3} colSpan={6} p='4'>
                        <Text>个人简介</Text>
                        <Textarea
                            value={introduction}
                            h='200px'
                            onChange={(e) => setIntroduction(e.target.value)}
                            errorBorderColor='crimson'
                            isInvalid={isIntroductionInvalid}
                        />
                    </GridItem>
                </Grid>
            </CardBody>
            <CardFooter>
                <Button size='lg' onClick={() => handleSubmit()} >完成</Button>
            </CardFooter>
        </Card >
    );
}