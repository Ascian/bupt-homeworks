'use client'

import UserIcon from '@/components/shared/userIcon';
import {
    Button,
    Card,
    CardBody,
    CardFooter,
    CardHeader,
    Stack,
    Grid,
    GridItem,
    Heading,
    Input,
    Text,
    Textarea,
    Alert,
    AlertIcon,
    AlertTitle,
    Link,
} from '@chakra-ui/react';
import { useToast } from '@chakra-ui/toast';
import { useSession } from 'next-auth/react';
import { useState } from 'react';
import config from '@/app/config'

export default function Page() {
    const [show, setShow] = useState(false);
    const { data: session, update } = useSession();

    const [phoneNumber, setPhoneNumber] = useState('');
    const [introduction, setIntroduction] = useState('');

    const [isPhoneNumberInvalid, setIsPhoneNumberInvalid] = useState(false);
    const [isIntroductionInvalid, setIsIntroductionInvalid] = useState(false);


    const toast = useToast()
    const handleSubmit = async () => {
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
            phoneNumber: phoneNumber,
            bio: introduction,
        }))
        const res = await fetch(`${config.serverIp}/users/me`, {
            method: 'PATCH',
            body: JSON.stringify({
                phoneNumber: phoneNumber,
                bio: introduction,
            }),
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${session.accessToken}`
            }
        })
        const user = await res.json()

        // If no error and we have user data, return it
        if (res.ok && user) {
            await update({
                ...session,
                user: {
                    ...session.user,
                    phoneNumber: user.phoneNumber,
                    introduction: user.bio,
                }
            })
            toast({
                title: '修改成功',
                status: 'success',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
        // Return null if user data could not be retrieved
        else {
            toast({
                title: '修改失败',
                description: `state: ${res.status}, message: ${user.message}`,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
    };

    return (
        <>
            {session ? (
                <Card align='center'>
                    <CardHeader>
                        <Stack align='center'>
                            <UserIcon boxSize='100px' />
                            <Heading size='lg' >{session.user.username}</Heading>
                        </Stack>
                    </CardHeader >

                    <CardBody>
                        <Grid templateRows='repeat(8, 1fr)' templateColumns='repeat(6, 1fr)' gap={1}>
                            <GridItem rowSpan={1} colSpan={3} p='4'>
                                <Text>密码</Text>
                                <Link href='/user/password'>
                                    <Button colorScheme='blue' >修改密码</Button>
                                </Link>
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={3} p='4'>
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={3} p='4'>
                                <Text>真实姓名</Text>
                                <Input variant='filled' isReadOnly='true' value={session.user.realName} />
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={3} p='4'>
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={3} p='4'>
                                <Text>证件类型</Text>
                                <Input variant='filled' isReadOnly='true' value={session.user.identityType} />
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={3} p='4'>
                                <Text>证件号</Text>
                                <Input variant='filled' isReadOnly='true' value={session.user.identityNumber} />
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={3} p='4'>
                                <Text>电话号码</Text>
                                <Input
                                    value={phoneNumber}
                                    placeholder={session.user.phoneNumber}
                                    onChange={(e) => setPhoneNumber(e.target.value)}
                                    errorBorderColor='crimson'
                                    isInvalid={isPhoneNumberInvalid}
                                />
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={3} p='4'>
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={2} p='4'>
                                <Text>国家</Text>
                                <Input variant='filled' isReadOnly='true' value={session.user.country} />
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={2} p='4'>
                                <Text>省份</Text>
                                <Input variant='filled' isReadOnly='true' value={session.user.province} />
                            </GridItem>
                            <GridItem rowSpan={1} colSpan={2} p='4'>
                                <Text>市</Text>
                                <Input variant='filled' isReadOnly='true' value={session.user.city} />
                            </GridItem>
                            <GridItem rowSpan={3} colSpan={6} p='4'>
                                <Text>个人简介</Text>
                                <Textarea
                                    value={introduction}
                                    placeholder={session.user.introduction}
                                    h='200px'
                                    onChange={(e) => setIntroduction(e.target.value)}
                                    errorBorderColor='crimson'
                                    isInvalid={isIntroductionInvalid}
                                />
                            </GridItem>
                        </Grid>
                    </CardBody>
                    <CardFooter>
                        <Button size='lg' onClick={() => handleSubmit()}>确认修改</Button>
                    </CardFooter>
                </Card >
            ) : (
                <Card align='center'>
                    <Alert status='error'>
                        <AlertIcon />
                        <AlertTitle>请先登录!</AlertTitle>
                    </Alert>
                </Card >
            )
            }
        </>
    );
}