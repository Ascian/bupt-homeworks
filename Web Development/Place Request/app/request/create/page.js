'use client'

import {
    Box,
    Alert,
    AlertIcon,
    AlertTitle,
    Card,
    CardBody,
    CardHeader,
    CardFooter,
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
import { useState, useRef } from 'react';
import { useToast } from '@chakra-ui/toast';
import { ChevronDownIcon } from '@chakra-ui/icons'
import { useSession } from 'next-auth/react';
import config from '@/app/config';

export default function Page() {
    const { data: session } = useSession();
    const hiddenFileInput = useRef(null);

    const [title, setTitle] = useState('');
    const [description, setDescription] = useState('');
    const [destinationType, setDestinationType] = useState('')
    const [maxExpectedPrice, setMaxExpectedPrice] = useState(0);
    const [seekerExpiryDate, setSeekerExpiryDate] = useState('');
    const [pictureLoc, setPictureLoc] = useState(null)

    const [isTitleInvalid, setIsTitleInvalid] = useState(false);
    const [isDescriptionInvalid, setIsDescriptionInvalid] = useState(false);
    const [isMaxExpectedPriceInvalid, setIsMaxExpectedPriceInvalid] = useState(false);
    const [isSeekerExpiryDateInvalid, setIsSeekerExpiryDateInvalid] = useState(false);

    const toast = useToast()

    const handlePictureSubmit = event => {
        hiddenFileInput.current.click();
    };

    const handleChange = event => {
        const fileUploaded = event.target.files[0];
        setPictureLoc(fileUploaded);
    };

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

        if (!pictureLoc) {
            toast({
                title: '图片不合法',
                description: "图片不能为空",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }

        const validImageTypes = ['image/jpeg', 'image/png'];
        if (!validImageTypes.includes(pictureLoc.type)) {
            toast({
                title: '图片不合法',
                description: "图片格式必须为jpg或png",
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }

        const formData = new FormData();
        formData.append('file', pictureLoc);
        const pictureRes = await fetch(`${config.serverIp}/files/upload`, {
            method: 'POST',
            body: formData,
        })
        const url = await pictureRes.text();
        if (!pictureRes.ok || !url) {
            toast({
                title: '图片上传失败',
                description: `${url.errorMessage}`,
                status: 'error',
                duration: 9000,
                isClosable: true,
            })
            return;
        }


        const dateObject = new Date(seekerExpiryDate);
        const date = dateObject.toISOString();

        const res = await fetch(`${config.serverIp}/seekers`, {
            method: 'POST',
            body: JSON.stringify({
                seekerTitle: title,
                seekerDescription: description,
                destinationType: destinationType,
                maxExpectedPrice: parseInt(maxExpectedPrice, 10),
                seekerExpiryDate: date,
                attachmentUrl: url,
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
                description: `${request.errorMessage}`,
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
                            <Heading size='lg' >创建请求</Heading>
                        </CardHeader>

                        <CardBody w='700px'>
                            <Text>标题</Text>
                            <Input
                                value={title}
                                onChange={(e) => setTitle(e.target.value)}
                                errorBorderColor='crimson'
                                isInvalid={isTitleInvalid}
                            />
                            <Box h='4' />
                            <Text>描述</Text>
                            <Textarea
                                h='200px'
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
                                        {destinationType}
                                    </Flex>
                                </MenuButton>
                                <MenuList maxHeight="200px" overflowY="auto">
                                    {config.destinationTypes.map((type) => (
                                        <MenuItem onClick={() => setDestinationType(type)} key={type}>{type}</MenuItem>
                                    ))}
                                </MenuList>
                            </Menu>
                            <Box h='4' />
                            <Text>最大期望价格</Text>
                            <NumberInput min={0} max={999999}
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
                                value={seekerExpiryDate}
                                onChange={(e) => setSeekerExpiryDate(e.target.value)}
                                errorBorderColor='crimson'
                                isInvalid={isSeekerExpiryDateInvalid}
                                type="date"
                            />
                            <Box h='4' />
                            <Text>展示图片</Text>
                            <Flex alignItems="center">
                                <Button colorScheme='facebook' size='md' onClick={handlePictureSubmit}>选择文件</Button>
                                <input
                                    type="file"
                                    ref={hiddenFileInput}
                                    onChange={handleChange}
                                    style={{ display: 'none' }}
                                />
                                <Box w='4' />
                                <Text>{pictureLoc ? pictureLoc.name : '未选择文件'}</Text>
                            </Flex>
                        </CardBody>
                        <CardFooter>
                            <Button colorScheme='facebook' size='lg' onClick={() => handleSubmit()} >提交</Button>
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