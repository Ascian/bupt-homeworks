'use client'

import {
    Box,
    Alert,
    AlertIcon,
    AlertTitle,
    Card,
    Grid,
    CardBody,
    CardHeader,
    CardFooter,
    GridItem,
    Heading,
    Input,
    Text,
    Textarea,
    Button,
    Flex,
    Menu,
    MenuButton,
    MenuList,
    MenuItem,
    NumberInput,
    NumberInputField,

} from '@chakra-ui/react';
import { useEffect, useState } from 'react';
import { useToast } from '@chakra-ui/toast';
import { ChevronDownIcon } from '@chakra-ui/icons'
import { useSession } from 'next-auth/react';
import config from '@/app/config';

export default async function ModifyRequest({ requestId }) {
    const { data: session } = useSession();
    const [request, setRequest] = useState({});
    const [requestSeekerExpiryDate, setRequestSeekerExpiryDate] = useState('');
    const [isLoading, setIsLoading] = useState(true);
    const toast = useToast()

    const [title, setTitle] = useState('');
    const [description, setDescription] = useState('');
    const [destinationType, setDestinationType] = useState('')
    const [maxExpectedPrice, setMaxExpectedPrice] = useState(0);
    const [seekerExpiryDate, setSeekerExpiryDate] = useState('');

    const [isTitleInvalid, setIsTitleInvalid] = useState(false);
    const [isDescriptionInvalid, setIsDescriptionInvalid] = useState(false);
    const [isMaxExpectedPriceInvalid, setIsMaxExpectedPriceInvalid] = useState(false);
    const [isSeekerExpiryDateInvalid, setIsSeekerExpiryDateInvalid] = useState(false);

    useEffect(() => {
        fetch(`${config.serverIp}/seekers/${requestId}`, {
            method: 'GET',
            headers: {
                "Content-Type": "application/json",
            },
        }).then((res) => res.json())
            .then((request) => {
                if (request) {
                    setRequest(request);
                    setRequestSeekerExpiryDate(new Date(request.seekerExpiryDate).toISOString().split('T')[0]);
                    setIsLoading(false);
                }
            })
    }, [requestId])

    if (isLoading) {
        return (<>
            <Spinner
                thickness='4px'
                speed='0.65s'
                emptyColor='gray.200'
                color='blue.500'
                size='xl'
            />
        </>);
    }

    const handleSubmit = async () => {
        if (title === '' || title.length > 20) {
            toast({
                title: '标题不合法',
                description: "标题不能为空且长度不能超过20",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsTitleInvalid(true);
            return;
        }
        if (description === '' || description.length > 10000) {
            toast({
                title: '描述不合法',
                description: "描述不能为空且长度不能超过10000",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsDescriptionInvalid(true);
            return;
        }
        if (maxExpectedPrice < 0 || maxExpectedPrice > 1000000) {
            toast({
                title: '最大期望价格不合法',
                description: "最大期望价格必须在0到1000000之间",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsMaxExpectedPriceInvalid(true);
            return;
        }
        if (seekerExpiryDate === '') {
            toast({
                title: '求购截止日期不合法',
                description: "求购截止日期不能为空",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            setIsSeekerExpiryDateInvalid(true);
            return;
        }

        const dateObject = new Date(seekerExpiryDate);
        const date = dateObject.toISOString();

        const res = await fetch(`${config.serverIp}/seekers/${requestId}`, {
            method: 'PATCH',
            body: JSON.stringify({
                seekerTitle: title,
                seekerDescription: description,
                destinationType: destinationType,
                maxExpectedPrice: parseInt(maxExpectedPrice, 10),
                seekerExpiryDate: date,
            }),
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${session.accessToken}`
            }
        })
        const request = await res.json()
        if (!res.ok || !request) {
            toast({
                title: '创建失败',
                description: `state: ${res.status}, message: ${request.message}`,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }
        else {
            toast({
                title: '创建成功',
                description: '即将跳转至请求页面',
                status: 'success',
                duration: 9000,
                isClosable: true,
            })
            const timer = setTimeout(() => {
                window.location.href = `/request?request_id=${request.seekerId}`
            }, 2000)

            return;
        }
    }

    return (
        <>
            {session ? (
                <>
                    <Card align='center' w='800px' h='auto'>
                        <CardHeader>
                            <Heading size='lg' >修改请求</Heading>
                        </CardHeader>

                        <CardBody w='700px'>
                            <Text>标题</Text>
                            <Input
                                placeholder={request.seekerTitle}
                                value={title}
                                onChange={(e) => setTitle(e.target.value)}
                                errorBorderColor='crimson'
                                isInvalid={isTitleInvalid}
                            />
                            <Box h='4' />
                            <Text>描述</Text>
                            <Textarea
                                h='200px'
                                placeholder={request.seekerDescription}
                                value={description}
                                onChange={(e) => setDescription(e.target.value)}
                                errorBorderColor='crimson'
                                isInvalid={isDescriptionInvalid}
                            />
                            <Box h='4' />
                            <Text>目的地类型</Text>
                            <Menu>
                                <MenuButton w='200px' as={Button} rightIcon={<ChevronDownIcon />} >
                                    <Flex justify='left'>
                                        {destinationType ? destinationType : request.destinationType}
                                    </Flex>
                                </MenuButton>
                                <MenuList maxHeight="200px" overflowY="auto">
                                    {config.destinationTypes.map((type) => (
                                        <MenuItem onClick={() => setDestinationType(type)}>{type}</MenuItem>
                                    ))}
                                </MenuList>
                            </Menu>
                            <Box h='4' />
                            <Text>最大期望价格</Text>
                            <NumberInput min={0} max={999999}
                                placeholder={request.maxExpectedPrice}
                                value={maxExpectedPrice}
                                onChange={(e) => setMaxExpectedPrice(e)}
                                errorBorderColor='crimson'
                                isInvalid={isMaxExpectedPriceInvalid}
                            >
                                <NumberInputField />
                            </NumberInput>
                            <Box h='4' />
                            <Text>请求截止日期</Text>
                            <Input
                                placeholder={requestSeekerExpiryDate}
                                value={seekerExpiryDate}
                                onChange={(e) => setSeekerExpiryDate(e.target.value)}
                                errorBorderColor='crimson'
                                isInvalid={isSeekerExpiryDateInvalid}
                                type="date"
                            />
                        </CardBody>
                        <CardFooter>
                            <Button colorScheme='telegram' size='lg' onClick={() => handleSubmit()} >提交</Button>
                        </CardFooter>
                    </Card>
                </>
            ) : (
                <Card align='center'>
                    <Alert status='error'>
                        <AlertIcon />
                        <AlertTitle>请先登录！</AlertTitle>
                    </Alert >
                </Card >
            )
            }
        </>
    );
}